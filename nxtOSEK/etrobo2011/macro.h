#ifndef __MACRO_H__
#define __MACRO_H__

/**
 *	配列要素数取得
 *
 *	配列の要素数を取得する。
 *
 * @param[in] array 配列名。ポインタではないこと。
 * @return 配列の要素数
 */
#define COUNTOF(array) (sizeof(array) / sizeof((array)[0]))

/**
 * 絶対値を取得する。
 *
 * @param[in] value 値
 * @return 絶対値
 */
#define ABS(value) ((value) < 0 ? -(value) : (value))

/**
 * 符号を取得する。
 *
 * @param[in] value 値
 * @return 符号
 */
#define SIGN(value) ((value) > 0 ? 1 : -1)

/**
 * 大きい方の値を取得する。
 *
 * @param[in] a 値１
 * @param[in] b 値２
 * @return 大きい方の値
 */
#define MAX(a,b) (((a)>(b))?(a):(b))

/**
 * 小さい方の値を取得する。
 *
 * @param[in] a 値１
 * @param[in] b 値２
 * @return 小さい方の値
 */
#define MIN(a,b) (((a)<(b))?(a):(b))

#endif // __MACRO_H__
