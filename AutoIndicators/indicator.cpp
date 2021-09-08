#include "includes.h"

namespace Indicator {

	inline float SMA(std::vector<double> _source, int _len) {

		std::vector<double> source;
		int n = _source.size();
		for (int i = 0; i < _len; i++) {
			source.push_back(_source[n - i - 1]);
		}

		float _sum = 0;
		for (auto& n : source)
			_sum += n;

		return _sum / _len;
	}

	inline SMAData SMA(std::vector<double> _source, SMAData lengths) {

		SMAData _return = {};
		float source = 0.0;
		int n = _source.size();

		for (int i = 0; i < lengths.biggest(); i++) {

			source += _source[n - i - 1];

			if (i == lengths.short_len - 1) {
				_return.short_len = source / lengths.short_len;
			}

			if (i == lengths.long_len - 1) {
				_return.long_len = source / lengths.long_len;
			}

			if (i == lengths.base_len - 1) {
				_return.base_len = source / lengths.base_len;
			}
		}

		return _return;
	}
}