#ifndef STROKEEFFECTSETTINGSPANEL_H
#define STROKEEFFECTSETTINGSPANEL_H

#include <QWidget>

#include "../../Model/Effects/SFBStrokeEffect.h"

namespace Ui {
class StrokeEffectSettingsPanel;
}

class StrokeEffectSettingsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit StrokeEffectSettingsPanel(QWidget *parent = 0);
    ~StrokeEffectSettingsPanel();

    void setValue(const SFBStrokeEffectSettings & value);
    SFBStrokeEffectSettings getValue() const;

signals:
    void valueChanged(SFBStrokeEffectSettings value);

private slots:
    void subWidgetValueChanged();

private:
    Ui::StrokeEffectSettingsPanel *ui;
    bool mIsUpdatingGui;
    SFBStrokeEffectSettings mValue;

};

#endif // STROKEEFFECTSETTINGSPANEL_H
