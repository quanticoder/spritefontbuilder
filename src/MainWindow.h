#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QtGui>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QComboBox>
#include <QMenu>

#include "Model/SFBDocument.h"

#include "UI/Widgets/InputSettingsPanel.h"
#include "UI/Widgets/GenerationSettingsPanel.h"
#include "UI/Widgets/FillEffectSettingsPanel.h"
#include "UI/Widgets/ShadowEffectSettingsPanel.h"
#include "UI/Widgets/StrokeEffectSettingsPanel.h"
#include "UI/Widgets/EffectListRow.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

    /** Overload close event. */
    void closeEvent(QCloseEvent *);

    /** Create a new document and load it. */
    void loadNewDocument();

    /** Set the window to use the provided document and lose the current document. */
    void setDocument(SFBDocument::Ptr document);

    /** Populate the combo boxes and setup any UI elements that need extra work. */
    void initializeGui();

    /** Refresh the texture atlas image. */
    void updateAtlasImage();

    /** Update Effect Row Previews. */
    void updateEffectRowPreviews();

    /** Recreate GUI panels for the loaded document. */
    void recreateGuiFromDocument();

    /** Update the GUI fields from the loaded document. */
    void updateGuiFromDocument();


private:
    /** Simply returns the index of the effect currently selected in the UI. */
    int selectedEffectIndex();

    /** Returns a brush which is used to fill the background area of the atlas graphics view. */
    QBrush atlasBackgroundBrush();


private slots:
    /** Update the loaded document from the GUI fields. */
    void updateDocumentFromGui();

    /** Triggered whenever the selected effect changes. */
    void selectedEffectChanged();

    /** Triggered whenever the effects are reordered. */
    void effectListRowsMoved(const QModelIndex & sourceParent, int sourceStart, int sourceEnd, const QModelIndex & destinationParent, int destinationRow);

    /** Triggered whenever an effect list row is right clicked and requests a popup menu. */
    void effectListRowContextMenu(const QPoint & pos);

    void on_actionExport_PNG_triggered();

    void on_actionToggle_Retina_Support_triggered();

    void on_toolButtonZoomIn_clicked();

    void on_toolButtonZoomOut_clicked();

    void on_toolButtonZoomReset_clicked();

    void on_actionSave_triggered();

    void on_actionOpen_triggered();

    void on_actionNew_triggered();

    void on_actionAdd_Fill_triggered();

    void on_actionAdd_Shadow_triggered();

    void on_actionAdd_Stroke_triggered();

    void on_actionDelete_Selected_Effect_triggered();

    void on_actionSaveAs_triggered();

    void on_actionDuplicate_Selected_Effect_triggered();

    void on_actionExport_triggered();

    void on_actionToggle_Glyph_Outlines_triggered();

private:

    // Member Variables
    float mCurrentZoom;
    bool mRetinaEnabled;
    bool mRenderGlyphRects;
    Ui::MainWindow *ui;
    QGraphicsScene mGraphicsScene;
    SFBDocument::Ptr mDocument;
    bool mSuppressAtlasUpdate;

    QVector<EffectListRow *> mEffectListRows;
    QVector<QWidget *> mEffectPanels;

    // Context menu for effect panel headers
    QMenu mEffectContextMenu;
};

#endif // MAINWINDOW_H
