#include "includes.h"

std::map<std::string, SavedData> Threads::BestPrices = {};
std::map<std::string, std::map<std::string, bool>> all_combination = {};

using namespace rapidjson;

namespace Threads {
	inline void Setup() {

		srand(time(NULL));
		for (const auto& entry : std::filesystem::directory_iterator(Settings::prices)) {

			std::string s_pth = entry.path().u8string();
			BestPrices[s_pth] = SavedData(s_pth);

			std::map<std::string, bool> done_combinations = {};
			all_combination[s_pth] = done_combinations;

			for (int i = 0; i < Settings::no_threads; i++) {
				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)GuessCheck, (LPVOID)&s_pth, NULL, NULL);
				std::this_thread::sleep_for(std::chrono::milliseconds(150));
			}

		}

	}

	inline void GuessCheck(LPVOID _price) {

		std::string price = *static_cast<std::string*>(_price);

		std::string content = Helper::ReadFile(price.c_str());
		printf("[THREAD] Started Input Loop: %s\n", Helper::ShortenPath(price).c_str());

		bool old_save = true;
		SavedData saved_data = SavedData(price);

		Document d;
		d.Parse(content.c_str());


		while (true) {
			try {
				clock_t tStart = clock();

				int E = 5;

				int short_len = Helper::RandomNumber(5, 30);
				int long_len = Helper::RandomNumber(10, 50);
				int base_len = Helper::RandomNumber(5, 70);

				std::vector<int> combination = { short_len, long_len, base_len };
				if (all_combination[price].find(Helper::ConvertNumbers(combination)) != all_combination[price].end()) {

					for (int ix = 5; ix <= 100; ix++) {
						for (int iy = 10; iy <= 100; iy++) {
							for (int iz = 15; iz <= 150; iz++) {

								combination = { ix, iy, iz };
								if (all_combination[price].find(Helper::ConvertNumbers(combination)) == all_combination[price].end()) {

									short_len = ix;
									long_len = iy;
									base_len = iz;

									goto theEnd;
								}
							}
						}
					}

					Console::Errors.push_back(ErrorLogs(Logging::getCurrentDateTime("now"),
						(Helper::ShortenPath(price) + ": ALL COMBINATIONS DONE!")));

					ExitThread(NULL);

				theEnd:
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
				all_combination[price][Helper::ConvertNumbers(combination)] = true;

				Inputs inputs = Inputs(
					E,
					short_len,
					long_len,
					base_len,
					true
				);

				if (old_save) {
					inputs = GetOldInputs(price);
					old_save = !old_save;
				}

				saved_data = BestPrices[price];
				Results results = Readings(price, inputs, d);
				if (results.percentage() < 30) {
					inputs = inputs.inverse();
					results = Readings(price, inputs, d);
				}

				saved_data.thread.variations++;
				if (results.percentage() > saved_data.highest_per &&
					results.total_trades() > Settings::min_trades &&
					//results.profit() > 0 &&

					results.first_time.percentage() > Settings::min_first_per &&
					results.second_time.percentage() > Settings::min_second_per &&
					results.whole_time.percentage() > Settings::min_whole_per) {

					saved_data.highest_per = results.percentage();

					Logging::LogFiles("[%s] %d [%.2f] - %d %d %d %d", Helper::ShortenPath(price).c_str(), results.total_trades(), results.percentage(), E, short_len, long_len, base_len);

					saved_data = SavedData(price, results, inputs, saved_data.thread, results.percentage());
					BestPrices[price] = saved_data;
				}

				//printf("[%s] %dT [%.2f] - %d\n", Helper::ShortenPath(price).c_str(), results.total_trades(), results.percentage(), rsi_len);
				//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

				BestPrices[price] = saved_data;

			}
			catch (const std::exception& exc) {
				Console::Errors.push_back(ErrorLogs(Logging::getCurrentDateTime("now"), exc.what()));
			}
		}
	}

	inline Results Readings(std::string price, Inputs inputs, Document& d) {

		Data data;
		Results _return;

		const Value& p = d["period"];

		_return.period = p.GetInt();
		int first_time = Settings::first_time_length / p.GetInt();
		int second_time = Settings::second_time_length / p.GetInt();
		int whole_time = Settings::whole_time_length / p.GetInt();

		const Value& a = d["close"];
		const int max_loops = whole_time + second_time + first_time + 200;

		const Value& t = d["time"];
		_return.time_stamp = t[t.Size() - 1].GetInt();

		bool first_exe = true;

		double short_sma = 0;
		double long_sma = 0;
		double base_sma = 0;

		for (int i = a.Size() - max_loops; i < a.Size(); i++) {

			data.candle_list.push_back(a[i].GetFloat());
			if (data.candle_list.size() < inputs.biggest())
				continue;
			else
				data.candle_list.erase(data.candle_list.begin());
			
			if (first_exe) {
				short_sma = Helper::AddPrice(data.candle_list, inputs.short_len);
				long_sma = Helper::AddPrice(data.candle_list, inputs.long_len);
				base_sma = Helper::AddPrice(data.candle_list, inputs.base_len);

				first_exe = !first_exe;
				continue;
			}
			else {
				int size = data.candle_list.size() - 1;

				short_sma = short_sma - data.candle_list[size - inputs.short_len] + data.candle_list[size];
				long_sma = long_sma - data.candle_list[size - inputs.long_len] + data.candle_list[size];
				base_sma = base_sma - data.candle_list[size - inputs.base_len] + data.candle_list[size];
			}

			data.shortSMA.push_back(short_sma / inputs.short_len);
			data.longSMA.push_back(long_sma / inputs.long_len);
			data.baseSMA.push_back(base_sma / inputs.base_len);


			if (data.shortSMA.size() < inputs.biggest() + 1)
				continue;
			else {
				data.shortSMA.erase(data.shortSMA.begin());
				data.longSMA.erase(data.longSMA.begin());
				data.baseSMA.erase(data.baseSMA.begin());
			}

			bool FIRST_CONDITIONS = Helper::CrossOver(data.shortSMA, data.longSMA) &&
				data.baseSMA[data.baseSMA.size() - 1] < data.shortSMA[data.shortSMA.size() - 1] &&
				data.baseSMA[data.baseSMA.size() - 1] < data.longSMA[data.longSMA.size() - 1] &&
				data.baseSMA[data.baseSMA.size() - 1] < data.candle_list[data.candle_list.size() - 1];
			
			bool SECOND_CONDITIONS = Helper::CrossOver(data.longSMA, data.shortSMA) &&
				data.baseSMA[data.baseSMA.size() - 1] > data.shortSMA[data.baseSMA.size() - 1] &&
				data.baseSMA[data.baseSMA.size() - 1] > data.longSMA[data.baseSMA.size() - 1] &&
				data.baseSMA[data.baseSMA.size() - 1] > data.candle_list[data.baseSMA.size() - 1];;

			if (inputs.inversed) {
				data.CALL.push_back(SECOND_CONDITIONS == 1);
				data.PUT.push_back(FIRST_CONDITIONS == 1);
			}
			else {
				data.CALL.push_back(FIRST_CONDITIONS == 1);
				data.PUT.push_back(SECOND_CONDITIONS == 1);
			}

			if (data.CALL.size() < 10 && data.PUT.size() < 10)
				continue;
			else {
				data.CALL.erase(data.CALL.begin());
				data.PUT.erase(data.PUT.begin());
			}

			int RESULTS = inputs.E + 1;
			bool WIN = (data.CALL[data.CALL.size() - RESULTS] && data.candle_list[data.candle_list.size() - RESULTS] < data.candle_list[data.candle_list.size() - 1]) || 
				(data.PUT[data.PUT.size() - RESULTS] && data.candle_list[data.candle_list.size() - RESULTS] > data.candle_list[data.candle_list.size() - 1]);

			bool LOSE = (data.CALL[data.CALL.size() - RESULTS] && data.candle_list[data.candle_list.size() - RESULTS] >= data.candle_list[data.candle_list.size() - 1]) || 
				(data.PUT[data.PUT.size() - RESULTS] && data.candle_list[data.candle_list.size() - RESULTS] <= data.candle_list[data.candle_list.size() - 1]);

			int loop = i - (a.Size() - max_loops);

			if (LOSE) {


				if (loop > 0 && loop < whole_time)
					_return.whole_time.wrong += 1;

				if (loop > whole_time && loop < whole_time + second_time)
					_return.second_time.wrong += 1;

				if (loop > whole_time + second_time)
					_return.first_time.wrong += 1;

			}

			if (WIN) {

				if (loop > 0 && loop < whole_time)
					_return.whole_time.right += 1;

				if (loop > whole_time && loop < whole_time + second_time)
					_return.second_time.right += 1;

				if (loop > whole_time + second_time)
					_return.first_time.right += 1;

			}

		}

		return _return;

	}

	inline Inputs GetOldInputs(std::string price) {

		Inputs inputs = {};
		std::string content = Helper::ReadFile(Settings::best_input.c_str());
		if (content.size() < 5)
			return inputs;

		Document d;
		d.Parse(content.c_str());

		rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
		size_t sz = allocator.Size();

		if (d.HasMember(Helper::JsonString(Helper::ShortenPath(price), allocator))) {

			const Value& obj = d[Helper::ShortenPath(price).c_str()];
			const Value& input = obj["inputs"];

			inputs.E = input[0].GetInt();
			inputs.short_len = input[1].GetInt();
			inputs.long_len = input[2].GetInt();
			inputs.base_len = input[3].GetInt();
			inputs.inversed = obj["inversed"].GetBool();
		}

		return inputs;
	}

	inline void SaveReading() {

		Document d;
		d.SetObject();

		rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
		size_t sz = allocator.Size();

		for (const auto& [key, value] : BestPrices) {

			SavedData data = value;
			if (data.results.profit() < 0)
				continue;

			Value main(kObjectType); {

				Value pair(kArrayType); {
					pair.PushBack(data.inputs.E, allocator);
					pair.PushBack(data.inputs.short_len, allocator);
					pair.PushBack(data.inputs.long_len, allocator);
					pair.PushBack(data.inputs.base_len, allocator);
				}
				main.AddMember("inputs", pair, allocator);

				Value inverse(data.inputs.inversed);
				main.AddMember("inversed", inverse, allocator);

				Value period(data.results.period);
				main.AddMember("period", period, allocator);

				Value stats(kObjectType); {

					Value trades(data.results.total_trades());
					stats.AddMember("trades", trades, allocator);

					Value percentage(data.results.percentage());
					stats.AddMember("percentage", percentage, allocator);

					Value profits(data.results.profit());
					stats.AddMember("profits", profits, allocator);

					Value one_month(kObjectType); {

						Value trades(data.results.second_time.total_trades());
						one_month.AddMember("trades", trades, allocator);

						Value percentage(data.results.second_time.percentage());
						one_month.AddMember("percentage", percentage, allocator);

						Value profits(data.results.second_time.profit());
						one_month.AddMember("profits", profits, allocator);

					}
					stats.AddMember("second_time", one_month, allocator);


					Value two_weeks(kObjectType); {

						Value trades(data.results.first_time.total_trades());
						two_weeks.AddMember("trades", trades, allocator);

						Value percentage(data.results.first_time.percentage());
						two_weeks.AddMember("percentage", percentage, allocator);

						Value profits(data.results.first_time.profit());
						two_weeks.AddMember("profits", profits, allocator);

					}
					stats.AddMember("first_time", two_weeks, allocator);


				}
				main.AddMember("stats", stats, allocator);
			}

			d.AddMember(Helper::JsonString(Helper::ShortenPath(data.price), allocator), main, allocator);

		}

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		Helper::SaveFile(buffer.GetString(), Settings::best_input.c_str());

	}
}