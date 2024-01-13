#include "hlib/core.h"
#include "hlib/hstring.h"
#include "hlib/hparse.h"

bool f_similar(f64 a, f64 b) {
	return a/b > 0.999 && a/b < 1.001;
}

i32 main() {
	i64 itest;
	u64 utest;
	f64 ftest;

	assert(hparse_u64(STR("1"), &utest));
	assert(utest == 1);

	assert(hparse_u64(STR("122"), &utest));
	assert(utest == 122);

	assert(!hparse_u64(STR("a"), &utest));
	assert(!hparse_u64(STR(""), &utest));

	assert(hparse_i64(STR("+22"), &itest));
	assert(itest == 22);

	assert(hparse_i64(STR("22"), &itest));
	assert(itest == 22);

	assert(hparse_i64(STR("-221"), &itest));
	assert(itest == -221);

	assert(!hparse_i64(STR("+-221"), &itest));
	assert(!hparse_i64(STR(""), &itest));
	assert(!hparse_i64(STR("+"), &itest));
	assert(!hparse_i64(STR("-"), &itest));

	assert(hparse_f64(STR("22.5"), &ftest));
	assert(f_similar(ftest, 22.5));

	assert(hparse_f64(STR("-212.5"), &ftest));
	assert(f_similar(ftest, -212.5));

	assert(hparse_f64(STR("2e5"), &ftest));
	assert(f_similar(ftest, 2e5));

	assert(hparse_f64(STR("2.44e-5"), &ftest));
	assert(f_similar(ftest, 2.44e-5));

	assert(hparse_f64(STR("22.44E+5"), &ftest));
	assert(f_similar(ftest, 22.44E+5));

	assert(!hparse_f64(STR("+"), &ftest));
	assert(!hparse_f64(STR("-"), &ftest));
	assert(!hparse_f64(STR("1e"), &ftest));
	assert(!hparse_f64(STR("e23"), &ftest));

	(void) itest;
	(void) ftest;
}
