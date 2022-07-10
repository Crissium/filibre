#ifndef MPVPLAYERDIALOG_H
#define MPVPLAYERDIALOG_H

#include <QDialog>
#include <string>

namespace Ui {
class MpvPlayerDialog;
}

class MpvPlayerDialog : public QDialog
{
	Q_OBJECT

public:
	explicit MpvPlayerDialog(const std::string & videoFileName, QWidget *parent = nullptr);
	~MpvPlayerDialog();

private:
	Ui::MpvPlayerDialog *ui;

public slots:
	void seek(int pos);
	void pauseResume();
private slots:
	void setSliderRange(int duration);
};

#endif // MPVPLAYERDIALOG_H
