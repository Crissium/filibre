#ifndef MPVWIDGET_H
#define MPVWIDGET_H

#include <QOpenGLWidget>
#include <mpv/client.h>
#include <mpv/render_gl.h>
#include "qthelper.h"

class MpvWidget : public QOpenGLWidget
{
	Q_OBJECT
public:
	MpvWidget(QWidget * parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	~MpvWidget();
	void command(const QVariant& params);
	void setProperty(const QString& name, const QVariant& value);
	QVariant getProperty(const QString& name) const;

signals:
	void durationChanged(int value);
	void positionChanged(int value);

protected:
	void initializeGL() override;
	void paintGL() override;

private slots:
	void on_mpv_events();
	void maybeUpdate();

private:
	void handle_mpv_event(mpv_event * event);
	static void on_update(void * ctx);

	mpv_handle * mpv;
	mpv_render_context * mpv_gl;
};

#endif // MPVWIDGET_H
