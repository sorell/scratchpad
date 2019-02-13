#define TARGET_X86


#ifdef TARGET_X86
#include <stdio.h>
#else
#include <FastLED.h>
#endif

#define NUM_LEDS 260
#define DATA_PIN 9
#define MAX_BRIGHT 30


#ifdef TARGET_X86
#include <stdint.h>
struct CRGB {
	CRGB() : r(0), g(0), b(0) {}
	CRGB(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue) {}
	CRGB &operator = (CRGB const &rhs) { r = rhs.r; g = rhs.g; b = rhs.b; return *this; }
	uint8_t r;
	uint8_t g;
	uint8_t b;
};
#endif

static CRGB leds[NUM_LEDS];

void printLeds()
{
	for (int i=0; i<20; ++i) {
#ifdef TARGET_X86
		fprintf(stderr, "%d ", leds[i].r);
#endif		
	}
	fprintf(stderr, "\n");
}






class LedArrayForward {};
class LedArrayReverse {};





// template <>
// class LedArray {};

class LedArrayBase
{
protected:
	LedArrayBase(CRGB *begin, CRGB *end) : begin_(begin), end_(end) {}

protected:
#if 0
	class iterator {
		friend LedArrayBase;
	public:
		bool operator != (iterator const &rhs) const { return p_ != rhs.p_; }
		iterator &operator ++ () { p_++; return *this; }
		iterator &operator -- () { p_--; return *this; }
		CRGB &operator * () { return *p_; }
		void at() const { fprintf(stderr, "iterator at %d\n", (int) (p_ - leds)); }
	protected:
		iterator(CRGB *p) : p_(p) {}
	private:
		CRGB *p_;
	};

	class reverse_iterator : public iterator {
		friend LedArrayBase;
	public:
		reverse_iterator &operator ++ () { fprintf(stderr, "backw++\n"); LedArrayBase::iterator::operator --(); return *this; }
		reverse_iterator &operator -- () { LedArrayBase::iterator::operator ++(); return *this; }
	protected:
		reverse_iterator(CRGB *p) : iterator(p) {}
	};
#endif

	class IteratorBase {
	public:
		bool operator != (IteratorBase const &rhs) const { return p_ != rhs.p_; }
		CRGB *&operator * () { return p_; }
		void at() const { fprintf(stderr, "iterator at %d\n", (int) (p_ - leds)); }
	protected:
		IteratorBase(CRGB *p) : p_(p) {}
		CRGB *p_;
	};

	class ForwardIterator : public IteratorBase {
		friend LedArrayBase;
	public:
		ForwardIterator &operator ++ () { p_++; return *this; }
		ForwardIterator &operator -- () { p_--; return *this; }
	protected:
		ForwardIterator(CRGB *p) : IteratorBase(p) {}
	};

	class ReverseIterator : public IteratorBase {
		friend LedArrayBase;
	public:
		ReverseIterator &operator ++ () { p_--; return *this; }
		ReverseIterator &operator -- () { p_++; return *this; }
	protected:
		ReverseIterator(CRGB *p) : IteratorBase(p) {}
	};
protected:
	CRGB *const begin_;
	CRGB *const end_;
};




template <typename T = LedArrayForward>
class LedArray : public LedArrayBase
{
public:
	LedArray(CRGB *leds, uint8_t first, uint8_t amount) : LedArrayBase(leds + first, leds + first + amount) {}

	typedef LedArrayBase::ForwardIterator iterator;
	typedef LedArrayBase::ReverseIterator r_iterator;

	iterator begin() { return iterator(begin_); }
	iterator end() { return iterator(end_); }
	r_iterator r_begin() { return r_iterator(end_-1); }
	r_iterator r_end() { return r_iterator(begin_-1); }

};


template <>
class LedArray<LedArrayReverse> : public LedArrayBase
{
protected:
	// class iterator : public LedArrayBase::iterator {
	// 	friend LedArray<LedArrayReverse>;
	// public:
	// 	LedArrayBase::iterator &operator ++ () { return LedArrayBase::iterator::operator -- (); }
	// 	LedArrayBase::iterator &operator -- () { return LedArrayBase::iterator::operator ++ (); }
	// protected:
	// 	iterator(CRGB *p) : LedArrayBase::iterator(p) {}
	// };

public:
	LedArray(CRGB *leds, uint8_t first, uint8_t amount) : LedArrayBase(leds + first + amount - 1, leds + first - 1) {}

	iterator begin() { return iterator(begin_); }
	iterator end() { return iterator(end_); }
};


class LedSet
{

};


void doStuff()
{
	int deadman;
	LedArray<> bottomRow1(leds, 0, 5);
	LedArray<LedArrayReverse> topRow1(leds, 10, 5);

	// LedSet firstRow(leds, 0, 120);

	CRGB red(200, 0, 0);

	fprintf(stderr, "Modify bottomRow1\n");
	for (auto it = bottomRow1.begin(); it != bottomRow1.end(); ++it) {
		it.at();
		**it = red;
		red.r++;
	}
	printLeds();

	fprintf(stderr, "Modify bottomRow1 backwards\n");
	deadman = 10;
	for (auto it = bottomRow1.r_begin(); it != bottomRow1.r_end(); ++it) {
		it.at();
		**it = red;
		red.r++;
		if (--deadman == 0) return;
	}
	printLeds();

	fprintf(stderr, "Modify topRow1\n");
	for (auto it = topRow1.begin(); it != topRow1.end(); ++it) {
		it.at();
		**it = red;
		red.r++;
	}
	printLeds();
}

int main(void)
{
	for (int i=0; i<NUM_LEDS; ++i) {
		leds[i].r = leds[i].g = leds[i].b = i + 100;
	}
	printLeds();
	doStuff();
	return 0;
}



#if 0



class SquaredVal {
public:
	SquaredVal() {
		for (uint32_t i=0; i<sizeof(values)/sizeof(values[0]); ++i) {
			float f = (float) i / 255.0f;
			values[i] = (uint8_t) (f * f * 255.0f);
		}
	}
	uint8_t operator [] (unsigned int const idx) const {
		if (idx > sizeof(values)/sizeof(values[0])) {
			Serial.print("SquaredVal["); Serial.print(idx); Serial.println("] called\n");
			while (1);
		}
		return values[idx];
	}

private:
	uint8_t values[256];
};


static CRGB CrbgInit(uint8_t const r, uint8_t const g, uint8_t const b)
{
	return {.red = (uint8_t) ((float) r / 255.0f * (float) MAX_BRIGHT), 
		.green = (uint8_t) ((float) g / 255.0f * (float) MAX_BRIGHT), 
		.blue = (uint8_t) ((float) b / 255.0f * (float) MAX_BRIGHT)};
}


class ColorRange {
public:
	ColorRange(CRGB const &high, CRGB const &low) : 
		high_(high), low_(low),
		redInc_((float) (high.red - low.red) / 100.0f), 
		greenInc_((float) (high.green - low.green) / 100.0f),
		blueInc_((float) (high.blue - low.blue) / 100.0f) {}

	CRGB percent(float const pc) const {
		return {(uint8_t) (low_.red + (uint8_t) (redInc_ * pc)), (uint8_t) (low_.green + (uint8_t) (greenInc_ * pc)), (uint8_t) (low_.blue + (uint8_t) (blueInc_ * pc))};
	}

private:
	CRGB const high_;
	CRGB const low_;
	float const redInc_;
	float const greenInc_;
	float const blueInc_;
};


class Hilight {
public:
		Hilight() : diff_({10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 90, 80, 70, 60, 50, 40, 30, 20, 10}), size_(sizeof(diff_)/sizeof(diff_[0])) {
			// diff_[0] = 15; diff_[1] = 30; diff_[2] = 45; diff_[3] = 60; diff_[4] = 45; diff_[5] = 30; diff_[6] = 15;
		}
public:
	int8_t const diff_[19];
	int const size_;
};

static SquaredVal const squaredVal;

static CRGB const black = {.red = 0, .green = 0, .blue = 0};
static ColorRange const pink = {CrbgInit(255, 110, 199), CrbgInit(26, 11, 20)};
// static ColorRange const forest = {CrbgInit(57, 255, 20), CrbgInit(6, 26, 2)};
static ColorRange const forest = {CrbgInit(0x49, 0xE2, 0x0E), CrbgInit(0x55, 0x10, 0x33)};
static ColorRange const ocean = {CrbgInit(0, 180, 255), CrbgInit(0, 18, 26)};
static ColorRange const autumn = {CrbgInit(255, 0, 0), CrbgInit(255, 255, 0)};

static int8_t const colorInc = 2;


static bool stop = false;


class LedState {
public:
	LedState() : percent_(0), direction_(0) {}
	LedState(int8_t const percent, int8_t const direction) : percent_(percent), direction_(direction) {}

	void copy(CRGB &rgb, ColorRange const &range) const {
		rgb = range.percent(percent_);
	}
	void advance(void) {
		int8_t const res = percent_ + colorInc * direction_;
		if (res < 0) {
			direction_ = 1;
		}
		else if (res > 100) {
			direction_ = -1;
		}
		else {
			percent_ = res;
			return;
		}
		advance();
	}

private:
	int8_t percent_;
	int8_t direction_;
};

// static LedState ledState[NUM_LEDS];
static unsigned long const changeWindow = 5000;
static unsigned long nextChange;
static Hilight hilight;

void setup() {
	Serial.begin(115200);

	FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);

	// for (int i=0; i<NUM_LEDS; ++i) {
	// 	ledState[i] = {0, 1};
	// }
	// for (int i=0; i<min(NUM_LEDS, 8); ++i) {
	// 	ledState[i] = {(int8_t) (i * 10), 1};
	// }

	nextChange = millis() + changeWindow;
}


void loop() {
	static int hilightPos = -hilight.size_;
	static uint8_t c = 0;
	static ColorRange const *colors[] = {&forest, &autumn, &ocean, &pink};

	for (int i=0; i<NUM_LEDS; ++i) {
		// ledState[i].advance();
		// ledState[i].copy(leds[i], *colors[c]);
		leds[i] = colors[c]->percent(0);
	}

	for (int i=0; i<hilight.size_; ++i) {
		int const pos = hilightPos + i;
		if (pos >= 0  &&  pos < NUM_LEDS) {
			leds[pos] = colors[c]->percent(hilight.diff_[i]);
		}
	}
	if (++hilightPos == 40) {
		hilightPos = -hilight.size_;
		if (++c >= (uint8_t) (sizeof(colors) / sizeof(colors[0]))) {
			c = 0;
		}
	}

	FastLED.show();
	delay(30);

#if 0
	if ((int) (nextChange - millis()) <= 0) {
	Serial.println("Change");
		nextChange += changeWindow;
		if (++c >= (uint8_t) (sizeof(colors) / sizeof(colors[0]))) {
			c = 0;
		}
	}

	if (0) {
		for (int i=0; i<NUM_LEDS; ++i) {
			Serial.print(i);
			Serial.print(": ");
			Serial.print(leds[i].red);
			Serial.print(", ");
			Serial.print(leds[i].green);
			Serial.print(", ");
			Serial.println(leds[i].blue);
		}
		while (1);
	}
#endif
}
#endif
