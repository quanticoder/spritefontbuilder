#ifndef INPUTSETTINGSPANEL_H
#define INPUTSETTINGSPANEL_H

#include <QWidget>
#include <QFontDatabase>

#include "../../Model/SFBTypes.h"


namespace Ui {
class InputSettingsPanel;
}

class InputSettingsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit InputSettingsPanel(QWidget *parent = 0);
    ~InputSettingsPanel();

    void setValue(const SFBInputSettings & value);
    SFBInputSettings getValue() const;

signals:
    void valueChanged(SFBInputSettings value);

private slots:
    void fontFamilyChanged();
    void fontStyleChanged();
    void fontSizeChanged();
    void inputCharactersChanged();

    void on_buttonAscii_clicked();

    void on_buttonNehe_clicked();

private:
    Ui::InputSettingsPanel *ui;

    bool mEmitSignals;
    SFBInputSettings mValue;
    QFontDatabase mFontDatabase;
};

#endif // INPUTSETTINGSPANEL_H
