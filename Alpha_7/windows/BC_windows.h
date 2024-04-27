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