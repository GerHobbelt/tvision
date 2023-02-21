//
// part of Demo 
//

#pragma once

#if defined(BUILD_MONOLITHIC)

#ifdef __cplusplus
extern "C" {
#endif

	int tvision_mmenu_main(void);
	int tvision_palette_test_main(void);
	int tvision_tvdemo_main(int argc, const char** argv);
	int tvision_tvdir_main(int argc, const char** argv);
	int tvision_tvedit_main(int argc, const char** argv);
	int tvision_genphone_main(void);
	int tvision_genparts_main(void);
	int tvision_tvforms_main(void);
	int tvision_tvhc_main(int argc, const char** argv);
	int tvision_hello_main(void);
	int tvision_geninc_main(void);

#ifdef __cplusplus
}
#endif

#endif
