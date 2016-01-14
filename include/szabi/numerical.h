#ifndef SZABI_NUMERICAL_H_
#define SZABI_NUMERICAL_H_

namespace szabi
{
	namespace numerical
	{
		/// <summary>
		/// Determines if a number is in a range determined by a reference and a tolerance [R-T, R+T]
		/// </summary>
		/// <param name="value">The number to check</param>
		/// <param name="reference">Reference value</param>
		/// <param name="tolerance">Relative error</param>
		template <typename T>
		bool tolerance_fail(const T& value, const T& reference, const T& tolerance)
		{
			if (value + tolerance <= reference ||
				value - tolerance >= reference)
			{
				return true;
			}

			return false;
		}
	}
}

#endif /* SZABI_NUMERICAL_H_ */