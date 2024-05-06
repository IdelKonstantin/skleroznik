#ifndef _BC_WINDOWS_H_
#define _BC_WINDOWS_H_

class UIwindow {

public:

	using funct_t = void(*)();
	class builder;

	UIwindow(funct_t setup, funct_t drawHead, funct_t drawBody, funct_t drawTail, funct_t drawCanvas, funct_t markers, funct_t worker);
	void start() const;

private:

	funct_t m_setup;
	funct_t m_drawHead;
	funct_t m_drawBody;
	funct_t m_drawTail;
	funct_t m_drawCanvas;
	funct_t m_markers;
	funct_t m_worker;
};

class UIwindow::builder {

public:

	builder& setSetup(funct_t value);
	builder& setDrawHead(funct_t value);
	builder& setDrawBody(funct_t value);
	builder& setDrawTail(funct_t value);
	builder& setDrawCanvas(funct_t value);
	builder& setMarkers(funct_t value);
	builder& setWorker(funct_t value);

	UIwindow build() const;
	static UIwindow::builder makeWindow();

private:

	funct_t m_setup{nullptr};
	funct_t m_drawHead{nullptr};
	funct_t m_drawBody{nullptr};
	funct_t m_drawTail{nullptr};
	funct_t m_drawCanvas{nullptr};
	funct_t m_markers{nullptr};
	funct_t m_worker{nullptr};
};

#endif /* _BC_WINDOWS_H_ */


/************************************************************************

TFT_UNIT tft{TFT_CS, TFT_RS, TFT_MOSI, TFT_SCK, TFT_RESET, TFT_MISO};

void setup() {

    Serial.begin(115200);
    tft.begin();

    UIwindow window_1 {

    	mw::setup, 
    	mw::drawHead, 
    	mw::drawBody, 
    	mw::drawTail, 
    	mw::drawCanvas, 
    	mw::markers, 
    	mw::worker
    };

    auto window_2 = UIwindow::builder::makeWindow()
        .setDrawBody(mw::drawBody)
        .setWorker(mw::worker)
        .build();

    auto window_3 = UIwindow::builder::makeWindow()
        .setDrawTail(mw::drawTail)
        .setSetup(mw::setup)
        .build();

    window_1.start();
    window_2.start();
    window_3.start();
}

************************************************************************/