// (C) komunre 2025 LGPL-2.1 License

#ifndef OUTGAUGE_H
#define OUTGAUGE_H

#define OG_PORT 4444

// (C) Vulcan-dev 2023 MIT License
// STARTS HERE

// Outgauge Flags
#define OG_SHIFT       1           // key // N/A
#define OG_CTRL        2           // key // N/A
#define OG_TURBO       8192        // show turbo gauge
#define OG_KM          16384       // if not set - user prefers MILES
#define OG_BAR         32768       // if not set - user prefers PSI

// Light Flags
#define DL_SHIFT       1 << 0      // shift light
#define DL_FULLBEAM    1 << 1      // full beam
#define DL_HANDBRAKE   1 << 2      // handbrake
#define DL_PITSPEED    1 << 3      // pit speed limiter // N/A
#define DL_TC          1 << 4      // TC active or switched off
#define DL_SIGNAL_L    1 << 5      // left turn signal
#define DL_SIGNAL_R    1 << 6      // right turn signal
#define DL_SIGNAL_ANY  1 << 7      // shared turn signal // N/A
#define DL_OILWARN     1 << 8      // oil pressure warning
#define DL_BATTERY     1 << 9      // battery warning
#define DL_ABS         1 << 10     // ABS active or switched off
#define DL_SPARE       1 << 11     // N/A

// Outgauge
typedef struct Outgauge_t  {
	unsigned int   time;            // time in milliseconds (to check order)
	char           car[4];          // Car name
	unsigned short flags;           // Info (see OG_x below)
	char           gear;            // Reverse:0, Neutral:1, First:2...
	char           plid;            // Unique ID of viewed player (0 = none)
	float          speed;           // M/S
	float          rpm;             // RPM
	float          turbo;           // BAR
	float          engTemp;         // C
	float          fuel;            // 0 to 1
	float          oilPressure;     // BAR
	float          oilTemp;         // C
	unsigned int   dashLights;      // Dash lights available (see DL_x below)
	unsigned int   showLights;      // Dash lights currently switched on
	float          throttle;        // 0 to 1
	float          brake;           // 0 to 1
	float          clutch;          // 0 to 1
	char           display1[16];    // Usually Fuel
	char           display2[16];    // Usually Settings
	int            id;              // optional - only if OutGauge ID is specified
} Outgauge_t;


// General
//------------------------------------------------------------------------
#define PSI 14.504 // BAR->PSI
#define MPH 2.237  // M/S->MPH
#define KPH 3.6    // M/S->KMH


// ENDS HERE

void open_socket();
Outgauge_t* receive_data();

#endif