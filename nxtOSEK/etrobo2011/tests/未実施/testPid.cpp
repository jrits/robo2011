#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <float.h>
#include <math.h>
#include "PID.hpp"

void testPID()
{
		
	PID mPID(FLT_MAX, FLT_MAX, FLT_MAX, 10);
		float Y1 = mPID.control_dummy(10, 12.5);
		printf("Y1 = %f\n",Y1);
		assert( isinf(Y1) );
		printf("Y1 OK!\n");

	PID	mPID2(FLT_MAX, FLT_MAX, FLT_MAX, 10);
		float Y2 = mPID2.control_dummy(10.1, 10);
		printf("Y2 = %f\n",Y2);
		assert( abs(Y2) - 0.2 * (FLT_MAX) < 0.01 );
		printf("Y2 OK!\n");

	PID mPID3(FLT_MAX, FLT_MAX, 0, 10);
		float Y3 = mPID3.control_dummy(10, 12.5);
		printf("Y3 = %f\n",Y3);
		assert( isinf(Y3) );
		printf("Y3 OK!\n");

	PID mPID4(FLT_MAX, 0, FLT_MAX, 10);
		float Y4 = mPID4.control_dummy(10, 12.5);
		printf("Y4 = %f\n",Y4);
		assert( isinf(Y4) );
		printf("Y4 OK!\n");

	PID mPID5(FLT_MAX, 0, FLT_MAX, 10);
		float Y5 = mPID5.control_dummy(10.1, 10);
		printf("Y5 = %f\n",Y5);
		assert( abs(Y5) - 0.2 * (FLT_MAX) < 0.01 );
		printf("Y5 OK!\n");

	PID mPID6(0, FLT_MAX, FLT_MAX, 10);
		float Y6 = mPID6.control_dummy(10, 12.5);
		printf("Y6 = %f\n",Y6);
		assert( isinf(Y6) );
		printf("Y6 OK!\n");

	PID mPID7(FLT_MAX, 0, 0, 10);
		float Y7 = mPID7.control_dummy(10, 12.5);
		printf("Y7 = %f\n",Y7);
		assert( isinf(Y7) );
		printf("Y7 OK!\n");

	PID mPID8(0, FLT_MAX, 0, 10);
		float Y8 = mPID8.control_dummy(10, 12.5);
		printf("Y8 = %f\n",Y8);
		assert( isinf(Y8) );
		printf("Y8 OK!\n");

	PID mPID9(0, 0, FLT_MAX, 10);
		float Y9 = mPID9.control_dummy(10, 12.5);
		printf("Y9 = %f\n",Y9);
		assert( isinf(Y9) );
		printf("Y9 OK!\n");

	PID mPID10(-FLT_MAX, -FLT_MAX, -FLT_MAX, 10);
		float Y10 = mPID10.control_dummy(12.5, 10);
		printf("Y10 = %f\n",Y10);
		assert( isinf(Y10) );
		printf("Y10 OK!\n");

	PID mPID11(-FLT_MAX, -FLT_MAX, -FLT_MAX, 10);
		float Y11 = mPID11.control_dummy(10, 10.1);
		printf("Y11 = %f\n",Y11);
		assert( abs(Y11) - 0.2 * (FLT_MAX) < 0.01 );
		printf("Y11 OK!\n");

	PID mPID12(-FLT_MAX, -FLT_MAX, 0, 10);
		float Y12 = mPID12.control_dummy(12.5, 10);
		printf("Y12 = %f\n",Y12);
		assert( isinf(Y12) );
		printf("Y12 OK!\n");

	PID mPID13(-FLT_MAX, 0, -FLT_MAX, 10);
		float Y13 = mPID13.control_dummy(12.5, 10);
		printf("Y13 = %f\n",Y13);
		assert( isinf(Y13) );
		printf("Y13 OK!\n");

	PID mPID14(0, -FLT_MAX, -FLT_MAX, 10);
		float Y14 = mPID14.control_dummy(12.5, 10);
		printf("Y14 = %f\n",Y14);
		assert( isinf(Y14) );
		printf("Y14 OK!\n");

	PID mPID15(-FLT_MAX, 0, 0, 10);
		float Y15 = mPID15.control_dummy(12.5, 10);
		printf("Y15 = %f\n",Y15);
		assert( isinf(Y15) );
		printf("Y15 OK!\n");

	PID mPID16(-FLT_MAX, 0, 0, 10);
		float Y16 = mPID16.control_dummy(10, 10.1);
		printf("Y16 = %f\n",Y16);
		assert( abs(Y16) - 0.1 * (FLT_MAX) < 0.01 );
		printf("Y16 OK!\n");

	PID mPID17(0, -FLT_MAX, 0, 10);
		float Y17 = mPID17.control_dummy(12.5, 10);
		printf("Y17 = %f\n",Y17);
		assert( isinf(Y17) );
		printf("Y17 OK!\n");

	PID mPID18(0, 0, -FLT_MAX, 10);
		float Y18 = mPID18.control_dummy(12.5, 10);
		printf("Y18 = %f\n",Y18);
		assert( isinf(Y18) );
		printf("Y18 OK!\n");

	PID mPID19(-FLT_MAX, -FLT_MAX, 0, 10);
		float Y19 = mPID19.control_dummy(12.5, 10);
		printf("Y19 = %f\n",Y19);
		assert( isinf(Y19) );
		printf("Y19 OK!\n");

	PID mPID20(0, 0, 0, 10);
		float Y20 = mPID20.control_dummy(12.5, 10);
		printf("Y20 = %f\n",Y20);
		assert( abs(Y20) - 0 < 0.01 );
		printf("Y20 OK!\n");

//(buf‚ð‹L‰¯‚³‚¹‚é)///////////////////////////////////
	PID mPID21(10, 0, 0, 10);
	float Y21 = mPID21.control(10, 12.5);
	printf("Y21 = %f\n",Y21);
	assert( (Y21) - (-25.0) < 0.01 );
	printf("Y21 OK!\n");
	
	float Y22 = mPID21.control(10.5, 12.5);
	printf("Y22 = %f\n",Y22);
	assert( (Y22) - (-20.0) < 0.01 );
	printf("Y22 OK!\n");
			
	float Y23 = mPID21.control(11.0, 12.5);
	printf("Y23 = %f\n",Y23);
	assert( (Y23) - (-15.0) < 0.01 );
	printf("Y23 OK!\n");
	
	float Y24 = mPID21.control(11.5, 12.5);
	printf("Y24 = %f\n",Y24);
	assert( (Y24) - (-10.0) < 0.01 );
	printf("Y24 OK!\n");

	float Y25 = mPID21.control(12.0, 12.5);
	printf("Y25 = %f\n",Y25);
	assert( (Y25) - (-5.0) < 0.01 );
	printf("Y25 OK!\n");
	
	float Y26 = mPID21.control(12.5, 12.5);
	printf("Y26 = %f\n",Y26);
	assert( (Y26) - (0.0) < 0.01 );
	printf("Y26 OK!\n");

	float Y27 = mPID21.control(13.0, 12.5);
	printf("Y27 = %f\n",Y27);
	assert( (Y26) - (5.0) < 0.01 );
	printf("Y26 OK!\n");

	float Y28 = mPID21.control(14.0, 12.5);
	printf("Y28 = %f\n",Y28);
	assert( (Y28) - (15.0) < 0.01 );
	printf("Y28 OK!\n");

	float Y29 = mPID21.control(14.5, 12.5);
	printf("Y29 = %f\n",Y29);
	assert( (Y29) - (20.0) < 0.01 );
	printf("Y28 OK!\n");

	float Y30 = mPID21.control(15.0, 12.5);
	printf("Y30 = %f\n",Y30);
	assert( (Y30) - (25.0) < 0.01 );
	printf("Y30 OK!\n");
///////////////////////////////////////////////////////
		
		
	PID mPID31(0, 10, 0, 10);
	float Y31 = mPID31.control3(10, 12.5);
	printf("Y31 = %f\n",Y31);
	assert( (Y31) - (-25.0) < 0.01 );
	printf("Y31 OK!\n");

	float Y32 = mPID31.control3(10.5, 12.5);
	printf("Y32 = %f\n",Y32);
	assert( (Y32) - (-45.0) < 0.01 );
	printf("Y32 OK!\n");
			
	float Y33 = mPID31.control3(11.0, 12.5);
	printf("Y33 = %f\n",Y33);
	assert( (Y33) - (-60.0) < 0.01 );
	printf("Y33 OK!\n");
	
	float Y34 = mPID31.control3(11.5, 12.5);
	printf("Y34 = %f\n",Y34);
	assert( (Y34) - (-70.0) < 0.01 );
	printf("Y34 OK!\n");

	float Y35 = mPID31.control3(12.0, 12.5);
	printf("Y35 = %f\n",Y35);
	assert( (Y35) - (-75.0) < 0.01 );
	printf("Y35 OK!\n");
	
	float Y36 = mPID31.control3(12.5, 12.5);
	printf("Y36 = %f\n",Y36);
	assert( (Y36) - (-75.0) < 0.01 );
	printf("Y36 OK!\n");

	float Y37 = mPID31.control3(13.0, 12.5);
	printf("Y37 = %f\n",Y37);
	assert( (Y37) - (-70.0) < 0.01 );
	printf("Y37 OK!\n");

	float Y38 = mPID31.control3(14.0, 12.5);
	printf("Y38 = %f\n",Y38);
	assert( (Y38) - (-55.0) < 0.01 );
	printf("Y38 OK!\n");

	float Y39 = mPID31.control3(14.5, 12.5);
	printf("Y39 = %f\n",Y39);
	assert( (Y39) - (-35.0) < 0.01 );
	printf("Y39 OK!\n");

	float Y40 = mPID31.control3(15.0, 12.5);
	printf("Y40 = %f\n",Y40);
	assert( (Y40) - (-10.0) < 0.01 );
	printf("Y40 OK!\n");
///////////////////////////////////////////////////
	
	PID mPID41(0, 0, 10, 10);
	float Y41 = mPID41.control4(10, 12.5);
	printf("Y41 = %f\n",Y41);
	assert( (Y41) - (0.0) < 0.01 );
	printf("Y41 OK!\n");

	float Y42 = mPID41.control4(10.5, 12.5);
	printf("Y42 = %f\n",Y42);
	assert( (Y42) - (5.0) < 0.01 );
	printf("Y42 OK!\n");
			
	float Y43 = mPID41.control4(11.0, 12.5);
	printf("Y43 = %f\n",Y43);
	assert( (Y43) - (10.0) < 0.01 );
	printf("Y43 OK!\n");
	
	float Y44 = mPID41.control4(11.5, 12.5);
	printf("Y44 = %f\n",Y44);
	assert( (Y44) - (15.0) < 0.01 );
	printf("Y44 OK!\n");

	float Y45 = mPID41.control4(12.0, 12.5);
	printf("Y45 = %f\n",Y45);
	assert( (Y45) - (20.0) < 0.01 );
	printf("Y45 OK!\n");
	
	float Y46 = mPID41.control4(12.5, 12.5);
	printf("Y46 = %f\n",Y46);
	assert( (Y46) - (25.0) < 0.01 );
	printf("Y46 OK!\n");

	float Y47 = mPID41.control4(13.0, 12.5);
	printf("Y47 = %f\n",Y47);
	assert( (Y47) - (30.0) < 0.01 );
	printf("Y47 OK!\n");

	float Y48 = mPID41.control4(14.0, 12.5);
	printf("Y48 = %f\n",Y48);
	assert( (Y48) - (40.0) < 0.01 );
	printf("Y48 OK!\n");

	float Y49 = mPID41.control4(14.5, 12.5);
	printf("Y49 = %f\n",Y49);
	assert( (Y49) - (45.0) < 0.01 );
	printf("Y49 OK!\n");

	float Y50 = mPID41.control4(15.0, 12.5);
	printf("Y50 = %f\n",Y50);
	assert( (Y50) - (50.0) < 0.01 );
	printf("Y50 OK!\n");

//////////////////////////////////////////////////////
	
	PID mPID51(10, 15, 0, 10);
	float Y51 = mPID51.control5(10, 12.5);
	printf("Y51 = %f\n",Y51);
	assert( (Y51) - (-62.5) < 0.01 );
	printf("Y51 OK!\n");

	float Y52 = mPID51.control5(10.5, 12.5);
	printf("Y52 = %f\n",Y52);
	assert( (Y52) - (-87.5) < 0.01 );
	printf("Y52 OK!\n");
			
	float Y53 = mPID51.control5(11.0, 12.5);
	printf("Y53 = %f\n",Y53);
	assert( (Y53) - (-105.0) < 0.01 );
	printf("Y53 OK!\n");
	
	float Y54 = mPID51.control5(11.5, 12.5);
	printf("Y54 = %f\n",Y54);
	assert( (Y54) - (-115.0) < 0.01 );
	printf("Y54 OK!\n");

	float Y55 = mPID51.control5(12.0, 12.5);
	printf("Y55 = %f\n",Y55);
	assert( (Y55) - (-117.5) < 0.01 );
	printf("Y55 OK!\n");
	
	float Y56 = mPID51.control5(12.5, 12.5);
	printf("Y56 = %f\n",Y56);
	assert( (Y56) - (-112.5) < 0.01 );
	printf("Y56 OK!\n");

	float Y57 = mPID51.control5(13.0, 12.5);
	printf("Y57 = %f\n",Y57);
	assert( (Y57) - (-100.0) < 0.01 );
	printf("Y57 OK!\n");

	float Y58 = mPID51.control5(14.0, 12.5);
	printf("Y58 = %f\n",Y58);
	assert( (Y58) - (-67.5) < 0.01 );
	printf("Y58 OK!\n");

	float Y59 = mPID51.control5(14.5, 12.5);
	printf("Y59 = %f\n",Y59);
	assert( (Y59) - (-32.5) < 0.01 );
	printf("Y59 OK!\n");

	float Y60 = mPID51.control5(15.0, 12.5);
	printf("Y60 = %f\n",Y60);
	assert( (Y60) - (10.0) < 0.01 );
	printf("Y60 OK!\n");
	
////////////////////////////////////////////////////
	
	
	PID mPID61(10, 0, 15, 10);
	float Y61 = mPID61.control6(10, 12.5);
	printf("Y61 = %f\n",Y61);
	assert( (Y61) - (-62.5) < 0.01 );
	printf("Y61 OK!\n");

	float Y62 = mPID61.control6(10.5, 12.5);
	printf("Y62 = %f\n",Y62);
	assert( (Y62) - (-12.5) < 0.01 );
	printf("Y62 OK!\n");
			
	float Y63 = mPID61.control6(11.0, 12.5);
	printf("Y63 = %f\n",Y63);
	assert( (Y63) - (0.00) < 0.01 );
	printf("Y63 OK!\n");
	
	float Y64 = mPID61.control6(11.5, 12.5);
	printf("Y64 = %f\n",Y64);
	assert( (Y64) - (12.5) < 0.01 );
	printf("Y64 OK!\n");

	float Y65 = mPID61.control6(12.0, 12.5);
	printf("Y65 = %f\n",Y65);
	assert( (Y65) - (25.0) < 0.01 );
	printf("Y65 OK!\n");
	
	float Y66 = mPID61.control6(12.5, 12.5);
	printf("Y66 = %f\n",Y66);
	assert( (Y66) - (37.5) < 0.01 );
	printf("Y66 OK!\n");

	float Y67 = mPID61.control6(13.0, 12.5);
	printf("Y67 = %f\n",Y67);
	assert( (Y67) - (50.0) < 0.01 );
	printf("Y67 OK!\n");

	float Y68 = mPID61.control6(14.0, 12.5);
	printf("Y68 = %f\n",Y68);
	assert( (Y68) - (75.0) < 0.01 );
	printf("Y68 OK!\n");

	float Y69 = mPID61.control6(14.5, 12.5);
	printf("Y69 = %f\n",Y69);
	assert( (Y69) - (87.5) < 0.01 );
	printf("Y69 OK!\n");

	float Y70 = mPID61.control6(15.0, 12.5);
	printf("Y70 = %f\n",Y70);
	assert( (Y70) - (100.0) < 0.01 );
	printf("Y70 OK!\n");
	
	
/////////////////////////////////////////////////
	
	PID mPID71(0, 10, 15, 10);
	float Y71 = mPID71.control7(10, 12.5);
	printf("Y71 = %f\n",Y71);
	assert( (Y71) - (-25.0) < 0.01 );
	printf("Y71 OK!\n");

	float Y72 = mPID71.control7(10.5, 12.5);
	printf("Y72 = %f\n",Y72);
	assert( (Y72) - (-37.5) < 0.01 );
	printf("Y72 OK!\n");

	float Y73 = mPID71.control7(11.0, 12.5);
	printf("Y73 = %f\n",Y73);
	assert( (Y73) - (-45.0) < 0.01 );
	printf("Y73 OK!\n");

	float Y74 = mPID71.control7(11.5, 12.5);
	printf("Y74 = %f\n",Y74);
	assert( (Y74) - (-47.5) < 0.01 );
	printf("Y74 OK!\n");

	float Y75 = mPID71.control7(12.0, 12.5);
	printf("Y75 = %f\n",Y75);
	assert( (Y75) - (-45.0) < 0.01 );
	printf("Y75 OK!\n");

	float Y76 = mPID71.control7(12.5, 12.5);
	printf("Y76 = %f\n",Y76);
	assert( (Y76) - (-37.5) < 0.01 );
	printf("Y76 OK!\n");

	float Y77 = mPID71.control7(13.0, 12.5);
	printf("Y77 = %f\n",Y77);
	assert( (Y77) - (-25.0) < 0.01 );
	printf("Y77 OK!\n");

	float Y78 = mPID71.control7(14.0, 12.5);
	printf("Y78 = %f\n",Y78);
	assert( (Y78) - (5.0) < 0.01 );
	printf("Y78 OK!\n");

	float Y79 = mPID71.control7(14.5, 12.5);
	printf("Y79 = %f\n",Y79);
	assert( (Y79) - (32.5) < 0.01 );
	printf("Y79 OK!\n");

	float Y80 = mPID71.control7(15.0, 12.5);
	printf("Y80 = %f\n",Y80);
	assert( (Y80) - (65.0) < 0.01 );
	printf("Y80 OK!\n");
	
	////////////////////////////////////////////
	
	PID mPID81(10, 15, 20, 10);
	float Y81 = mPID81.control8(10, 12.5);
	printf("Y81 = %f\n",Y81);
	assert( (Y81) - (-62.5) < 0.01 );
	printf("Y81 OK!\n");

	float Y82 = mPID81.control8(10.5, 12.5);
	printf("Y82 = %f\n",Y82);
	assert( (Y82) - (-77.5) < 0.01 );
	printf("Y82 OK!\n");

	float Y83 = mPID81.control8(11.0, 12.5);
	printf("Y83 = %f\n",Y83);
	assert( (Y83) - (-85.0) < 0.01 );
	printf("Y83 OK!\n");

	float Y84 = mPID81.control8(11.5, 12.5);
	printf("Y84 = %f\n",Y84);
	assert( (Y84) - (-85.0) < 0.01 );
	printf("Y84 OK!\n");

	float Y85 = mPID81.control8(12.0, 12.5);
	printf("Y85 = %f\n",Y85);
	assert( (Y85) - (-77.5) < 0.01 );
	printf("Y85 OK!\n");

	float Y86 = mPID81.control8(12.5, 12.5);
	printf("Y86 = %f\n",Y86);
	assert( (Y86) - (-62.5) < 0.01 );
	printf("Y86 OK!\n");

	float Y87 = mPID81.control8(13.0, 12.5);
	printf("Y87 = %f\n",Y87);
	assert( (Y87) - (-40.0) < 0.01 );
	printf("Y87 OK!\n");

	float Y88 = mPID81.control8(14.0, 12.5);
	printf("Y88 = %f\n",Y88);
	assert( (Y88) - (12.5) < 0.01 );
	printf("Y88 OK!\n");

	float Y89 = mPID81.control8(14.5, 12.5);
	printf("Y89 = %f\n",Y89);
	assert( (Y89) - (57.5) < 0.01 );
	printf("Y89 OK!\n");

	float Y90 = mPID81.control8(15.0, 12.5);
	printf("Y90 = %f\n",Y90);
	assert( (Y90) - (110.0) < 0.01 );
	printf("Y90 OK!\n");
	

}

int main (void)
{
    testPID();
    printf("ALL OK!\n");
	
	return 0;
}
