#ifndef ScEditStyleH
#define ScEditStyleH

/** Style (highligher) selected for Scintilla */
enum ScEditStyle {
	SC_STYLE_CPP = 0,
	SC_STYLE_JSON,
	SC_STYLE_LUA,

	SC_STYLE_LIMITER
};

enum ScEditCppColorScheme {
	SC_EDIT_CPP_COLOR_REGULAR = 0,
	SC_EDIT_CPP_COLOR_DARK,

	SC_EDIT_CPP_COLOR__LIMITER
};

enum ScEditJsonColorScheme {
	SC_EDIT_JSON_COLOR_REGULAR = 0,
	SC_EDIT_JSON_COLOR_DARK,

	SC_EDIT_JSON_COLOR__LIMITER
};


#endif
