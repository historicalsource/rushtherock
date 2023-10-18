
/*
rel_102
 */

/* GUTS commands */
#define S_STOP_ALL_SNDS	0x8000			/* stop all sounds */
#define S_ATTRACT_MODE	0x8001			/* select attract volume mode */
#define S_GAME_MODE	0x8002			/* select game volume mode */
#define S_VOLUME_TEST	0x8003			/* volume test: jefferson airplane */
#define S_SMAX_CALLS	0x8004			/* answer the number of sound calls available */
#define S_DO_DIAGNOSTICS	0x8005			/* start diagnostics (including checksum) */
#define S_SET_GAME_VOL	0x8006			/* set the game mode volume */
#define S_SET_ATTR_VOL	0x8007			/* set the attract mode volume */
#define S_MOS_VER	0x8008			/* answer the current MOS version number */
#define S_PROJECT_VER	0x8009			/* answer the current project version number */
#define S_ERROR_CNT	0x800A			/* answer the number of errors logged by MOS */
#define S_CLEAR_ERROR_LOG	0x800B			/* clear the error log */
#define S_RETURN_ERROR_LOG	0x800C			/* answer a per-error log */
#define S_REPORT_CHECKSUMS	0x800D			/* answer sixteen checksums */
#define S_PEEK	0x800E			/* answer 32-bit data at a 20-bit address */
#define S_SPEAKER_TEST	0x800F			/* start a Quad L/R test */
#define S_SINE_1K	0x8010			/* start a 1kHz sine wave */
#define S_KILL_SINE	0x8011			/* stop the sine wave */
#define S_TEST2	0x8012			/* start the second test */
#define S_KILL_TEST2	0x8013			/* stop the second test */
#define S_LOAD_BANK	0x8014			/* load a bank into memory from disk */
#define S_UNLOAD_BANK	0x8015			/* unload a bank from memory */
#define S_BANK_REPORT	0x8016			/* report the loaded banks */
#define S_MOOB_LISTENER	0x8019			/* set the MOOB listener */
#define S_MOOB_KILL_ALL	0x801A			/* kill all motion objects */
#define S_MOOB_KILL_OBJECT	0x801B			/* kill a motion object by handle */
#define S_MOOB_CHANGE_POSITION	0x801C			/* change the (Cartesian) position of a motion object by handle */
#define S_MOOB_CHANGE_VELOCITY	0x801D			/* change the velocity of a motion object by handle */
#define S_MOOB_CHANGE_PITCH	0x801E			/* change the position of a motion object by handle */
#define S_MOOB_CHANGE_FILTER	0x801F			/* change the filter control of a motion object by handle */
#define S_ENGINE_STOP            0x8022  /* stop the listener's engine */
#define S_ENGINE_CHANGE_PF       0x8023  /* change a pitch/filter engine */
#define S_ENGINE_CHANGE_PFV      0x8024  /* change a pitch/filter/volume engine */
#define S_ENGINE_CHANGE_AUTO     0x8025  /* change the speed of an automatic engine */
#define S_ENGINE_CRASH           0x8026  /* quickly change the speed of an automatic engine */

/* game-specific commands */
#define S_MULTIDUMMY		0x8030			/* Doppler Moob multiidle to match moob engine */
#define S_M_MULTIIDLE		0x8031			/* multi-idle as other car's engine, 7 args, rpm and load */
#define S_MULTIIDLE			0x8032			/* multi-idle as your own engine, 2 args, rpm and load */
#define S_AUDREY			0x8033			/* APM engine Audrey (rpm, load) */
#define S_NSX				0x8034			/* APM engine NSX (rpm, load) */
#define S_GHR				0x8035			/* APM engine HILOAD (rpm, load) */
#define S_TPM	       		0x8036  		/* APM engine tpm (rpm, load) */ /*cool I got the 69 command! */
#define S_REVERB_SEND_VOL	0x8037			/* reverb (send) */
#define S_REVERB_RETURN_VOL	0x8038			/* reverb (return) */
#define S_REVERB_FEEDBACK	0x8039			/* reverb feedback (aa,ab,bb,ba) */
#define S_REVERB_SIZE	    0x803A			/* reverb size (width,height) */
#define S_REVERB_PARMS	    0x803B			/* reverb set parms */

#define S_WINNER		0x803C			/* S_WINNER RPM */
#define S_KWINNER		0x803D			/* Kill S_WINNER */
#define S_SEAGULLS		0x803E			/* S_SEAGULLS RPM */
#define S_KSEAGULLS		0x803F			/* Kill S_SEAGULLS RPM */
#define S_EXPLO			0x8040			/* S_EXPLO RPM */
#define S_KEXPLO		0x8041			/* Kill S_EXPLO RPM */
#define S_WHATSYERNAME	0x8042			/* WHATSYERNAME RPM */
#define S_KWHATSYERNAME	0x8043			/* Kill WHATSYERNAME RPM */
#define S_PARKBIRD		0x8044			/* S_PARKBIRD RPM */
#define S_KPARKBIRD		0x8045			/* Kill PARKBIRD RPM */
#define S_SELECT		0x8046			/* Music to select cars by RPM */
#define S_KSELECT		0x8047			/* Kill SELECT RPM */
#define S_FANFARE2		0x8048			/* 3-2-1 ASCEND RPM */
#define S_GARAGEAMB		0x8049			/* Garageambienc RPM */
#define S_KGARAGEAMB	0x804A			/* Kill Garageamb RPM */

#define S_CARSELECT		0x804B			/* CarSelect RPM */
#define S_KCARSELECT	0x804C			/* Kill Carselect RPM */
#define S_FOGHORN		0x804D			/* FOGHORN RPM */
#define S_KFOGHORN		0x804E			/* Kill FOGHORN RPM */
#define S_LOWERVOL		0x804F			/* Lower volume of Select RPM */
#define S_AUG8ATT		0x8050			/* S_AUG8ATT */
#define S_KAUG8ATT		0x8051			/* Kill S_AUG8ATT */
#define S_PULPATT		0x8052			/* S_PULPATT */
#define S_KPULPATT		0x8053			/* Kill S_PULPATT */
#define S_STLATT		0x8054			/* S_STLATT */
#define S_KSTLATT		0x8055			/* Kill S_STLATT */
#define S_ZETHNOATT		0x8056			/* ZethnoATT */
#define S_KZETHNOATT	0x8057			/* Kill ZethnoATT */
#define S_RAVERUSHATT	0x8058			/* RAVERUSHATT */
#define S_KRAVERUSHATT	0x8059			/* Kill RAVERUSHATT */
#define S_BLUESATT		0x805A			/* BluesATT RPM */

#define S_KBLUESATT		0x805B			/* Kill BluesATT RPM */
#define S_PLAYITAGAIN	0x805C			/* PLAYITAGAIN RPM */
#define S_KPLAYITAGAIN	0x805D			/* Kill PLAYITAGAIN RPM */
#define S_WHATSTAG		0x805E			/* GOTO TAG FOR WHATSYERNAME RPM */

#define S_ANN1			0x805F			/* Moob  static up ANNOUNCER1 */
#define S_ANN2			0x8060			/* Moob  static up ANNOUNCER2 */
#define S_ANN3			0x8061			/* Moob  static up ANNOUNCER3 */
#define S_ANN4			0x8062			/* Moob  static up ANNOUNCER4 */
#define S_ANN5			0x8063			/* Moob  static up ANNOUNCER5 */
#define S_ANN6			0x8064			/* Moob  static up ANNOUNCER6 */
#define S_FINISHMOOB	0x8065			/* MOOB_ST_STATIC_UP CROWDFINISHMOOB */
#define S_CABLECARBELL	0x8066			/* MOOB_ST_STATIC_UP CABLECAR BELL */
#define S_FIRECRACKER	0x8067			/* MOOB_ST_STATIC_UP FIRECRACKER */
#define S_SMALLHOOT		0x8068			/* MOOB_ST_STATIC_UP SMALL CROWD YELLS */
#define S_SMALLCLAP		0x8069			/* MOOB_ST_STATIC_UP SMALL CROWD CLAPS */
#define S_PRISONMOOB	0x806A			/* MOOB_ST_STATIC_UP PRISON SIREN MOOB */
#define S_BLANK2		0x806B			/* MOOB_ST_STATIC_UP PLACEBO */

#define S_SKID_CUSTOM1	0x806C			/* SKID WITH 3 ARGS, HANDLE, PITCH & VOL */
#define S_SKID_CUSTOM2	0x806D			/* SKID W 3 ARGS, HANDLE, PITCH & VOL */
#define S_WINDNOISE		0x806E			/* WIND START AT 30 MPH HANDLE, PITCH, VOL */
#define S_ROADNOISE		0x806F			/* ROAD W 3 ARGS, HANDLE, PITCH, VOL */
#define S_GRAVELNOISE	0x8070			/* 3 ARGS HANDLE, PITCH, VOL */
#define S_WATERROAD		0x8071			/* 3 ARGS HANDLE, PITCH, VOL */
#define S_PEELOUT		0x8072			/* PEELOUT W 3 ARGS, HANDLE, PITCH, VOL */
#define S_SQUEAL_LEFT   0x8073 	  		/* Squeek sound for left tire */
#define S_SQUEAL_RIGHT  0x8074	  		/* Squeek sound for right tire */

#define S_CARBUMP		0x8075			/* MOOB_ST_POSITION, SMALL COLLISION */
#define S_CARSCRAPE		0x8076			/* MOOB_ST_POSITION, HIGH SPEED SCRAPING SOUND */
#define S_CARSMASH		0x8077			/* MOOB_ST_POSITION, MEDIUM INTENSITY COLLISION */
#define S_CURBWHUMP		0x8078			/* Moob st position CURB WHUMP */
#define S_BOTTOMOUT		0x8079			/* MOOB_ST_POSITION, VOL & ANGLE BOTTOMOUT */
#define S_CONES    		0x807A	  		/* Cones in the road st_position */
#define S_GLASS			0x807B	  		/* Plate glass window st_position*/
#define S_PMETER		0x807C  		/* Parking meter hits st_position */
#define S_SCRAPELOOP	0x807D	  		/* looped scraping sound st_position */
#define S_KSCRAPELOOP	0x807E	  		/* Kill looped scraping sound */
#define S_LIGHTPOLE	    0x807F	  		/* HIT A POLE st_position */
#define S_TREE	   		0x8080	  		/* HIT A TREE st_position */
#define S_FENCE	   		0x8081	  		/* HIT A FENCE st_position */
#define S_BOOM	   		0x8082	  		/* SEAT-SHAKING SOUND st_position */

#define S_CAR_LANDS		0x8083			/* CAR LANDS AFTER JUMP */
#define S_CHKPNTSTATIC	0x8084			/* Moob sfx "CHECKPOINT!" */
#define S_BEEP1			0x8085			/* Moob sfx BEEP1 time warning */
#define S_BOG			0x8086			/* Moob sfx ENGINE BOGGING */
#define S_KBOG			0x8087			/* KILL ENGINE BOGGING */
#define S_LEADERLIGHT	0x8088			/* Moob sfx GTR CHORD LEADER LIGHT */
#define S_KLEADERLIGHT	0x8089			/* KILL SFX GTR CHORD LEADER */
#define S_TRACKBROWSE	0x808A	  		/* BROWSE THE TRACKS SFX */

#define S_TRANSELECT	0x808B	  		/* SELECT TRANSMISSION SOUND SFX */
#define S_LIGHTSOUND	0x808C	  		/* Browse Cars SFX */
#define S_CROWDSCARE	0x808D	  		/* CROWDSCARE SFX */
#define S_TURNTABLE	    0x808E	  		/* looped CAR TURNTABLE SFX */
#define S_KTURNTABLE	0x808F	  		/* Kill TURNTABLE */
#define S_COIN1			0x8090  		/* Partial credit */
#define S_COIN2			0x8091	  		/* Full Credit */
#define S_THREE			0x8092	  		/* THREE COUNTDOWN */
#define S_TWO			0x8093	  		/* TWO COUNTDOWN */
#define S_ONE			0x8094	  		/* ONE COUNTDOWN */
#define S_RUSH			0x8095	  		/* RUSH! COUNTDOWN */
#define S_SPLASH		0x8096	  		/* SPLASH IN WATER */
#define S_DANGEROUS		0x8097	  		/* "IT'S DANGEROUS!" */
#define S_KLAXON		0x8098	  		/* EXTREME CAR WARNING HORN */
#define S_KKLAXON		0x8099	  		/* Kill EXTREME CAR WARNING HORN */
#define S_SPUTTER		0x809A	  		/* CAR SPUTTERS AT TIME OUT  */

#define S_KSPUTTER		0x809B	  		/* KILL CAR SPUTTERS  */
#define S_START1		0x809C	  		/* STARTING ANNOUNCER1 */
#define S_KSTART1		0x809D	  		/* KILL STARTING ANNOUNCER1 */
#define S_START2		0x809E	  		/* STARTING ANNOUNCER2 */
#define S_KSTART2		0x809F	  		/* KILL STARTING ANNOUNCER2 */
#define S_START3		0x80A0	  		/* STARTING ANNOUNCER3 */
#define S_KSTART3		0x80A1	  		/* KILL STARTING ANNOUNCER3 */
#define S_FINALL	 	0x80A2	  		/* FINAL LAP */
#define S_2L		 	0x80A3	  		/* 2 LAPS TO GO */
#define S_3L		 	0x80A4	  		/* 3 LAPS TO GO */
#define S_4L		 	0x80A5	  		/* 4 LAPS TO GO */
#define S_5L		 	0x80A6	  		/* 5 LAPS TO GO */
#define S_6L		 	0x80A7	  		/* 6 LAPS TO GO */
#define S_7L		 	0x80A8	  		/* 7 LAPS TO GO */
#define S_8L		 	0x80A9	  		/* 8 LAPS TO GO */
#define S_9L		 	0x80AA	  		/* 9 LAPS TO GO */

#define S_10L			0x80AB	  		/* extinct 10 LAPS TO GO */
#define S_11L			0x80AC	  		/* extinct 11 LAPS TO GO */
#define S_12L			0x80AD	  		/* extinct 12 LAPS TO GO */
#define S_13L	 		0x80AE	  		/* extinct 13 LAPS TO GO */
#define S_14L			0x80AF	  		/* extinct 14 LAPS TO GO */
#define S_15L 			0x80B0	  		/* extinct 15 LAPS TO GO */
#define S_16L			0x80B1	  		/* extinct 16 LAPS TO GO */
#define S_17L			0x80B2	  		/* extinct 17 LAPS TO GO */
#define S_18L			0x80B3	  		/* extinct 18 LAPS TO GO */
#define S_19L	 		0x80B4	  		/* extinct 19 LAPS TO GO */

#define S_BEEPCAR 		0x80B5	  		/* LIL' BEEP FOR CAR SELECT */
#define S_BEEPWARN1 	0x80B6	  		/* BEEPWARN1 BEGINNER CAR */
#define S_KBEEPWARN1 	0x80B7	  		/* KILL BEEPWARN1 BEGINNER CAR */
#define S_BEEPWARN2 	0x80B8	  		/* BEEPWARN2 ADVANCED CAR */
#define S_KBEEPWARN2 	0x80B9	  		/* KILL BEEPWARN2 ADVANCED CAR */
#define S_BEEPWARN3 	0x80BA	  		/* BEEPWARN3 EXPERT CAR */
#define S_KBEEPWARN3 	0x80BB	  		/* KILL BEEPWARN3 EXPERT CAR */
#define S_YOUOLD1	 	0x80BC	  		/* EXTINCT you have selected track1 */
#define S_YOUOLD2	 	0x80BD	  		/* EXTINCTyou have selected track2 */
#define S_YOUOLD3	 	0x80BE	  		/* EXTINCTyou have selected track3 */
#define S_WAITING1	 	0x80BF	  		/* EXTINCTwaiting for drivers track1 */
#define S_WAITING2	 	0x80C0	  		/* EXTINCTwaiting for drivers track2 */
#define S_WAITING3	 	0x80C1	  		/* EXTINCTwaiting for drivers track3 */
#define S_8WAY		 	0x80C2	  		/* exclamation for 8-way link */
#define S_2CARS		 	0x80C3	  		/* 2 CARS IN A POD */
#define S_3CARS		 	0x80C4	  		/* 3 CARS IN A POD */
#define S_4CARS		 	0x80C5	  		/* 4 CARS IN A POD */
#define S_5CARS		 	0x80C6	  		/* 5 CARS IN A POD */
#define S_6CARS		 	0x80C7	  		/* 6 CARS IN A POD */
#define S_7CARS		 	0x80C8	  		/* 7 CARS IN A POD */
#define S_8CARS		 	0x80C9	  		/* 8 CARS IN A POD */
#define S_K8CARS		0x80CA	  		/* KILL 8 CARS IN A POD */
#define S_K7CARS	 	0x80CB	  		/* KILL 7 CARS IN A POD */
#define S_K6CARS	 	0x80CC	  		/* KILL 6 CARS IN A POD */
#define S_K5CARS	 	0x80CD	  		/* KILL 5 CARS IN A POD */
#define S_K4CARS	 	0x80CE	  		/* KILL 4 CARS IN A POD */
#define S_K3CARS	 	0x80CF	  		/* KILL 3 CARS IN A POD */
#define S_K2CARS	 	0x80D0	  		/* KILL 2 CARS IN A POD */
#define S_LINKED	 	0x80D1	  		/* EXCLAMATION for 2-WAY LINK */
#define S_BUSH		 	0x80D2	  		/* HIT A BUSH moob st position */
#define S_RUSHWHISPER	0x80D3	  		/* Whispering encouragement for LEADER light */
#define S_CROWDRIGHT	0x80D4	  		/* STATIC CROWD ON THE RIGHT */
#define S_KCROWDRIGHT	0x80D5	  		/* KILL CROWD ON THE RIGHT */
#define S_CROWDLEFT		0x80D6	  		/* STATIC CROWD ON THE LEFT */
#define S_KCROWDLEFT	0x80D7	  		/* KILL CROWD ON THE LEFT */
		 								   
#define S_THEROCK		0x80D8	  		/* RPM tune for alcatraz */
#define S_KTHEROCK		0x80D9	  		/* kill RPM tune for alcatraz */
#define S_PRISONSIREN	0x80DA	  		/* SIREN for atmosphere moobSFX*/
#define S_KPRISONSIREN	0x80DB	  		/* KILL SIREN */
#define S_WAIT1			0x80DC	  		/* RPM waiting for drivers track1*/
#define S_WAIT2			0x80DD	  		/* RPM waiting for drivers track2*/
#define S_WAIT3			0x80DE	  	    /* RPM waiting for drivers track3*/
#define S_WAIT4			0x80DF	  	    /* RPM waiting for drivers track4*/
#define S_WAIT5			0x80E0	  	    /* RPM waiting for drivers track5*/
#define S_WAIT6			0x80E1	  	    /* RPM waiting for drivers track6*/
#define S_WAIT7			0x80E2	  	    /* RPM waiting for drivers track7*/
#define S_WAIT8			0x80E3	  	    /* RPM waiting for drivers track8*/
#define S_WAITROCK		0x80E4	  	    /* RPM waiting for drivers THE ROCK*/

#define S_YOU1			0x80E5	  	    /* RPM you have selected track1*/
#define S_YOU2			0x80E6	  	    /* RPM you have selected track2*/
#define S_YOU3			0x80E7   	    /* RPM you have selected track3*/
#define S_YOU4			0x80E8	  	    /* RPM you have selected track4*/
#define S_YOU5			0x80E9	  	    /* RPM you have selected track5*/
#define S_YOU6			0x80EA	  	    /* RPM you have selected track6*/
#define S_YOU7			0x80EB	  	    /* RPM you have selected track7*/
#define S_YOU8			0x80EC	  	    /* RPM you have selected track8*/
#define S_YOUROCK		0x80ED	  	    /* RPM you have selected THE ROCK*/

#define S_YELL			0x80EE	  	    /* Moob sfx Yell2 exclamation*/
#define S_ITSFAT		0x80EF	  	    /* Moob sfx Its Fat exclamation*/
#define S_CHIPS			0x80F0	  	    /* Moob sfx All that and a bag o' chips exclamation*/
#define S_YOUGOTTA		0x80F1	  	    /* Moob sfx I can't do anymore... exclamation*/
#define S_ITSINSANE		0x80F2	  	    /* Moob sfx It's insane exclamation*/
#define S_LOOKOUT		0x80F3  	    /* Moob sfx Lookout exclamation*/
#define S_BADLICIOUS	0x80F4	  	    /* Moob sfx It's Badlicious exclamation*/
#define S_WAVENET		0x80F5	  	    /* RPM wavenet tune*/
#define S_KWAVENET		0x80F6	  	    /* kill RPM wavenet tune*/
#define S_WAVETAG1		0x80F7	  	    /* RPM first wavenet tag*/
#define S_WAVETAG2		0x80F8	  	    /* RPM 2nd wavenet tag*/
#define S_TOURNEY1		0x80F9	  	    /* RPM Tournament play, go for the prize*/
#define S_REGIONAL		0x80FA	  	    /* RPM Regional winner, you've been promoted..*/
#define S_KREGIONAL		0x80FB	  	    /* kill RPM Regional*/
#define S_REGISTER1		0x80FC	  	    /* RPM "register to win!"*/
#define S_JAILDOORLOOP	0x80FD	  	    /* Moob sfx JAILDOOR LOOP*/
#define S_KJAILDOORLOOP	0x80FE	  	    /* Moob sfx KILL JAILDOOR LOOP*/
#define S_JAILDOORCLANG	0x80FF	  	    /* Moob sfx JAILDOOR CLANG*/
