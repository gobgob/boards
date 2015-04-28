// SPRITES
const sprite_t arrow_top = {
    {0x000000,0x000000,0x000000,0x841686,0x841686,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0x841686,0x841686,0x841686,0x841686,0x000000,0x000000},
    {0x000000,0x841686,0x000000,0x841686,0x841686,0x000000,0x841686,0x000000},
    {0x000000,0x000000,0x000000,0x841686,0x841686,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0x000000,0x841686,0x841686,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0x000000,0x841686,0x841686,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0x000000,0x841686,0x841686,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000}
};

const sprite_t heart = {
    {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0xff0000,0xff0000,0x000000,0xff0000,0xff0000,0x000000},
    {0x000000,0xff0000,0xff0000,0xff0000,0x000000,0xff0000,0xff0000,0xff0000},
    {0x000000,0xff0000,0xff0000,0xff0000,0xff0000,0xff0000,0xff0000,0xff0000},
    {0x000000,0xff0000,0xff0000,0xff0000,0xff0000,0xff0000,0xff0000,0xff0000},
    {0x000000,0x000000,0xff0000,0xff0000,0xff0000,0xff0000,0xff0000,0x000000},
    {0x000000,0x000000,0x000000,0xff0000,0xff0000,0xff0000,0x000000,0x000000},
    {0x000000,0x000000,0x000000,0x000000,0xff0000,0x000000,0x000000,0x000000}
};

const sprite_t clap_open = {
    {0x000000,0x000000,0x000000,0x000000,0xFFFFFF,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0x000000,0xFFFFFF,0x000000,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0xFFFFFF,0x000000,0x000000,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0x000000,0x000000},
    {0x000000,0x000000,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0x000000,0x000000},
    {0x000000,0x000000,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0x000000,0x000000},
    {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000}
};

const sprite_t clap_close = {
    {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0x000000,0x000000},
    {0x000000,0x000000,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0x000000,0x000000},
    {0x000000,0x000000,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0x000000,0x000000},
    {0x000000,0x000000,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0x000000,0x000000},
    {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000}
};

const sprite_t sprite_row = {
    {1,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0}
};

const sprite_t sprite_line = {
    {1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
};

const sprite_t lid01 = {
	{ 0xae8fad, 0xae8fad, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xae8fad, 0xae8fad },
	{ 0xae8fad, 0xffcf00, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xffcf00, 0xae8fad },
	{ 0xffcf00, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xffcf00 },
	{ 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad },
	{ 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad },
	{ 0xffcf00, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xffcf00 },
	{ 0xae8fad, 0xffcf00, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xffcf00, 0xae8fad },
	{ 0xae8fad, 0xae8fad, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xae8fad, 0xae8fad }
};

const sprite_t lid02 = {
	{ 0xae8fad, 0xae8fad, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xae8fad, 0xae8fad },
	{ 0xae8fad, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xae8fad },
	{ 0xffcf00, 0xffcf00, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xffcf00, 0xffcf00 },
	{ 0xffcf00, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xffcf00 },
	{ 0xffcf00, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xffcf00 },
	{ 0xffcf00, 0xffcf00, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xffcf00, 0xffcf00 },
	{ 0xae8fad, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xae8fad },
	{ 0xae8fad, 0xae8fad, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xae8fad, 0xae8fad } 
};

const sprite_t lid03 = {

	{ 0xae8fad, 0xae8fad, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xae8fad, 0xae8fad },
	{ 0xae8fad, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xae8fad },
	{ 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00 },
	{ 0xffcf00, 0xffcf00, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xffcf00, 0xffcf00 },
	{ 0xffcf00, 0xffcf00, 0xae8fad, 0xae8fad, 0xae8fad, 0xae8fad, 0xffcf00, 0xffcf00 },
	{ 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00 },
	{ 0xae8fad, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xae8fad },
	{ 0xae8fad, 0xae8fad, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xae8fad, 0xae8fad } 
};

const sprite_t lid04 = {
	{ 0xae8fad, 0xae8fad, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xae8fad, 0xae8fad },
	{ 0xae8fad, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xae8fad },
	{ 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00 },
	{ 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00 },
	{ 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00 },
	{ 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00 },
	{ 0xae8fad, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xae8fad },
	{ 0xae8fad, 0xae8fad, 0xffcf00, 0xffcf00, 0xffcf00, 0xffcf00, 0xae8fad, 0xae8fad } 
};

const sprite_t iris = {
	{ 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff },
	{ 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff },
	{ 0xffffff, 0xffffff, 0xffffff, 0x532704, 0x532704, 0xffffff, 0xffffff, 0xffffff },
	{ 0xffffff, 0xffffff, 0x532704, 0x000000, 0x000000, 0x532704, 0xffffff, 0xffffff },
	{ 0xffffff, 0xffffff, 0x532704, 0x000000, 0x000000, 0x532704, 0xffffff, 0xffffff },
	{ 0xffffff, 0xffffff, 0xffffff, 0x532704, 0x532704, 0xffffff, 0xffffff, 0xffffff },
	{ 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff },
	{ 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff }
};
