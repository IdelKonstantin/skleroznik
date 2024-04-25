using funct_t = void(*)();

namespace BC {
    
    static void setup() {
        Serial.println("setup");
    }
    
    static void drawHead() {
        Serial.println("drawHead");
    }
    
    static void drawBody() {
        Serial.println("drawBody");
    }
    
    static void drawTail() {
        Serial.println("drawTail");
    }
    
    static void drawCanvas() {
        Serial.println("drawCanvas");
    }
    
    static void markers() {
        Serial.println("markers");
    }
    
    static void worker() {
        Serial.println("worker");
    }
}

class UIwindow {

private:

    using funct_t = void(*)();

    funct_t m_setup;
    funct_t m_drawHead;
    funct_t m_drawBody;
    funct_t m_drawTail;
    funct_t m_drawCanvas;
    funct_t m_markers;
    funct_t m_worker;

public:
    UIwindow(funct_t setup, funct_t drawHead, funct_t drawBody, funct_t drawTail, funct_t drawCanvas, funct_t markers, funct_t worker) : 

    m_setup(setup),
    m_drawHead(drawHead),
    m_drawBody(drawBody),
    m_drawTail(drawTail),
    m_drawCanvas(drawCanvas),
    m_markers(markers),
    m_worker(worker)
    {
        Serial.println("===========");

        if(m_setup) {
            m_setup();
        }
        if(m_drawHead) {
            m_drawHead();
        }
        if(m_drawBody) {
            m_drawBody();
        }
        if(m_drawTail) {
            m_drawTail();
        }
        if(m_drawCanvas) {
            m_drawCanvas();
        }
        if(m_markers) {
            m_markers();
        }
        if(m_worker) {
            m_worker();
        }
    }

    class builder;
};

class UIwindow::builder {

public:
    builder& setSetup(funct_t value) {m_setup = value; return *this;};
    builder& setDrawHead(funct_t value) {m_drawHead = value; return *this;};
    builder& setDrawBody(funct_t value) {m_drawBody = value; return *this;};
    builder& setDrawTail(funct_t value) {m_drawTail = value; return *this;};
    builder& setDrawCanvas(funct_t value) {m_drawCanvas = value; return *this;};
    builder& setMarkers(funct_t value) {m_markers = value; return *this;};
    builder& setWorker(funct_t value) {m_worker = value; return *this;};

    UIwindow build() const {
        return {m_setup, m_drawHead, m_drawBody, m_drawTail, m_drawCanvas, m_markers, m_worker};
    }

    static UIwindow::builder make() {
        return UIwindow::builder{};
    } 

private:

    funct_t m_setup{nullptr};
    funct_t m_drawHead{nullptr};
    funct_t m_drawBody{nullptr};
    funct_t m_drawTail{nullptr};
    funct_t m_drawCanvas{nullptr};
    funct_t m_markers{nullptr};
    funct_t m_worker{nullptr};
};

void setup() {

  Serial.begin(115200);

  UIwindow window_1{BC::setup, BC::drawHead, BC::drawBody, BC::drawTail, BC::drawCanvas, BC::markers, BC::worker};
  
  auto window_2 = UIwindow::builder::make()
        .setDrawBody(BC::drawBody)
        .setWorker(BC::worker)
        .build();

  auto window_3 = UIwindow::builder::make()
        .setDrawTail(BC::drawTail)
        .setSetup(BC::setup)
        .build();
}

void loop() {


}
