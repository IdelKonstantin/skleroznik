#include "../../inc/BC_windows.h"

UIwindow::UIwindow(funct_t setup, funct_t drawHead, funct_t drawBody, funct_t drawTail, funct_t drawCanvas, funct_t markers, funct_t worker) : 

	m_setup(setup),
	m_drawHead(drawHead),
	m_drawBody(drawBody),
	m_drawTail(drawTail),
	m_drawCanvas(drawCanvas),
	m_markers(markers),
	m_worker(worker) {}

void UIwindow::start() const {

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

/////////////////////////////////////////////////////////////////////////////////////////////////

UIwindow::builder& UIwindow::builder::setSetup(funct_t value) {

	m_setup = value; 
	return *this;
};

UIwindow::builder& UIwindow::builder::setDrawHead(funct_t value) {

	m_drawHead = value; 
	return *this;
};

UIwindow::builder& UIwindow::builder::setDrawBody(funct_t value) {

	m_drawBody = value; 
	return *this;
};

UIwindow::builder& UIwindow::builder::setDrawTail(funct_t value) {
	
	m_drawTail = value; 
	return *this;
};

UIwindow::builder& UIwindow::builder::setDrawCanvas(funct_t value) {
	
	m_drawCanvas = value; 
	return *this;
};

UIwindow::builder& UIwindow::builder::setMarkers(funct_t value) {
	
	m_markers = value; 
	return *this;
};

UIwindow::builder& UIwindow::builder::setWorker(funct_t value) {

	m_worker = value; 
	return *this;
};

UIwindow UIwindow::builder::build() const {
	return {m_setup, m_drawHead, m_drawBody, m_drawTail, m_drawCanvas, m_markers, m_worker};
}

UIwindow::builder UIwindow::builder::makeWindow() {
	return UIwindow::builder{};
}