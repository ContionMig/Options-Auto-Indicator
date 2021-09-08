#pragma once
#include "includes.h"

std::vector<ErrorLogs> Console::Errors = {};

namespace Console {
	inline void Setup() {
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Print, NULL, NULL, NULL);
	}


	inline void Print() {

		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::string started_time = Logging::getCurrentDateTime("now");
		clock_t tStart = clock();

		while (true) {

			WaterMark();

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3); {
				printf("Started Running: %s\n", started_time.c_str());
				TextTable t('-', '|', '+'); {

					t.add("Pair");

					t.add("-");

					t.add("Trades");
					t.add("Trades/Day");

					t.add("-");

					t.add("Pft");

					t.add("-");

					t.add("Avg. Pft");

					t.add("-");

					t.add("Pft/2 Mth");
					t.add("Pft/1 Mth");
					t.add("Pft/1 Wk");

					t.add("-");

					t.add("Expiry");
					t.add("Short");
					t.add("Long");
					t.add("Base");
					t.add("Inverse");

					t.add("-");

					t.add("Loops");

					t.endOfRow();

					for (const auto& [key, value] : Threads::BestPrices) {

						SavedData data = value;

						t.add(Helper::ShortenPath(data.price) + " ");

						t.add("-");

						t.add(std::to_string(data.results.total_trades()));
						t.add(Helper::Round((float)data.results.total_trades() / (data.results.total_time() / 60.0 / 24.0)) + "/$" + Helper::Round(data.results.profit() / data.results.total_trades()));

						t.add("-");

						t.add("$" + Helper::Round(data.results.profit()));

						t.add("-");

						t.add(Helper::Round(data.results.percentage()) + "%");

						t.add("-");

						t.add(Helper::Round(data.results.whole_time.percentage()) + "%");
						t.add(Helper::Round(data.results.second_time.percentage()) + "%");
						t.add(Helper::Round(data.results.first_time.percentage()) + "%");

						t.add("-");

						t.add(std::to_string(data.inputs.E) + "M");
						t.add(std::to_string(data.inputs.short_len));
						t.add(std::to_string(data.inputs.long_len));
						t.add(std::to_string(data.inputs.base_len));
						t.add(data.inputs.inversed ? "True" : "False");

						t.add("-");

						t.add(std::to_string(data.thread.variations));

						t.endOfRow();

					}

					std::cout << t;
				}
			}


			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); {
				printf("Time Elapsed: %.2f hours\n\n", (double)(clock() - tStart) / CLOCKS_PER_SEC / 60 / 60);
			}

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5); {
				printf("Runtime Logs:\n");
				for (int i = 0; i < Errors.size(); i++) {
					printf("[%s] %s\n", Errors[i].time.c_str(), Errors[i].error.c_str());
				}
			}

			Threads::SaveReading();
			std::this_thread::sleep_for(std::chrono::seconds(30));
			system("cls");
		}
	}

	inline void WaterMark() {

		system("cls");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
		printf("\n");
		printf(" ___  _ _  ___  ___   _  _ _  ___  _  ___  ___  ___  ___  ___    _______  ___  / /_(_)__  ___  __ _  (_)__ _ \n");
		printf("| . || | ||_ _|| . | | || \\ || . \\| ||  _>| . ||_ _|| . || . \\  / __/ _ \\/ _ \\/ __/ / _ \\/ _ \\/  ' \\/ / _ `/ \n");
		printf("|   || ' | | | | | | | ||   || | || || <__|   | | | | | ||   /  \\__/\\___/_//_/\\__/_/\\___/_//_/_/_/_/_/\\_, /  \n");
		printf("|_|_|`___' |_| `___' |_||_\\_||___/|_|`___/|_|_| |_| `___'|_\\_\\                                       /___/   \n");
		printf("\n");
		printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
		printf("\n");

	}
}