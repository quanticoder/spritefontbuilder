#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QCheckBox>

#include "Model/Exporters/SFBFontExporters.h"
#include "Model/SFBFile/SFBFileWriter.h"
#include "Model/SFBFile/SFBFileReader.h"

#include "UI/Widgets/EffectListRow.h"

static const char * APP_TITLE = "Sprite Font Builder";
static const char * APP_VERSION = "0.8.0";

#if defined(Q_OS_MAC)
    static const char * APP_PLATFORM = "OSX";
#elif defined(Q_OS_WIN)
    static const char * APP_PLATFORM = "Windows";
#elif defined(Q_OS_LINUX)
    static const char * APP_PLATFORM = "Linux";
#else
    static const char * APP_PLATFORM = "Unknown";
#endif


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mRenderGlyphRects(false)
{
    ui->setupUi(this);
    initializeGui();

    loadNewDocument();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if ( mDocument->needsSaving() )
    {
        int result = QMessageBox::question(this, "Save Changes?", "Do you want to save the changes you made to this Sprite Font?", "Don't Save", "Cancel", "Save");

        if ( result == 0 )
        {
            event->accept();
        }
        else if ( result == 1 )
        {
            event->ignore();
        }
        else if ( result == 2 )
        {
            on_actionSave_triggered();
            event->accept();
        }
    }
    else
    {
        event->accept();
    }
}


void MainWindow::loadNewDocument()
{
    SFBDocument::Ptr document = SFBDocument::Ptr(new SFBDocument());
    document->initDefaultDocument();
    setDocument(document);
}


void MainWindow::setDocument(SFBDocument::Ptr document)
{
    mDocument = document;

    recreateGuiFromDocument();
    updateGuiFromDocument();
    updateAtlasImage();
}


void MainWindow::initializeGui()
{
    setWindowTitle(QString("%1 - %2 (%3)").arg(APP_TITLE, APP_VERSION, APP_PLATFORM));
    mSuppressAtlasUpdate = true;

    ui->atlasGraphicsView->setBackgroundBrush(QBrush(QColor(100,100,100,255)));
    ui->atlasGraphicsView->setScene(&mGraphicsScene);

    mCurrentZoom = 1.0f;
    mRetinaEnabled = true;

    QObject::connect(ui->inputSettingsPanel, SIGNAL(valueChanged(SFBInputSettings)), this, SLOT(updateDocumentFromGui()));
    QObject::connect(ui->generationSettingsPanel, SIGNAL(valueChanged(SFBGenerationSettings)), this, SLOT(updateDocumentFromGui()));
    QObject::connect(ui->exportSettingsPanel, SIGNAL(valueChanged(SFBExportSettings)), this, SLOT(updateDocumentFromGui()));
    QObject::connect(ui->listWidgetEffects, SIGNAL(itemSelectionChanged()), this, SLOT(selectedEffectChanged()));
    QObject::connect(ui->listWidgetEffects->model(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(effectListRowsMoved(QModelIndex,int,int,QModelIndex,int)));

    mSuppressAtlasUpdate = false;
}


void MainWindow::recreateGuiFromDocument()
{
    mSuppressAtlasUpdate = true;

    // Clear Existing UI
    while( ui->stackedEffects->count() > 0 ) {
        ui->stackedEffects->removeWidget(ui->stackedEffects->widget(0));
    }

    mEffectPanels.clear();
    mEffectListRows.clear();
    ui->listWidgetEffects->clear();

    // Create Title Rows
    for( int i = 0; i < mDocument->getEffectCount(); ++i )
    {
        SFBEffect * effect = mDocument->getEffectAtIndex(i).get();
        EffectListRow * row = new EffectListRow(this, effect->getTitle());
        QObject::connect(row, SIGNAL(checkedStateChanged(bool)), this, SLOT(updateDocumentFromGui()));
        QObject::connect(row, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(effectListRowContextMenu(QPoint)));

        QListWidgetItem * item = new QListWidgetItem();
        ui->listWidgetEffects->addItem(item);
        item->setSizeHint(row->minimumSize());

        mEffectListRows.push_back(row);
        ui->listWidgetEffects->setItemWidget(item, row);
    }

    // Effect Panels
    for( int i = 0; i < mDocument->getEffectCount(); ++i )
    {
        SFBEffect * effect = mDocument->getEffectAtIndex(i).get();
        SFBEffectType effectType = effect->getEffectType();
        QWidget * panel = nullptr;

        if ( effectType == SFBEffectType::Effect_Fill )
        {
            FillEffectSettingsPanel * fillPanel = new FillEffectSettingsPanel(this);
            QObject::connect(fillPanel, SIGNAL(valueChanged(SFBFillEffectSettings)), this, SLOT(updateDocumentFromGui()));
            panel = fillPanel;
        }
        else if ( effectType == SFBEffectType::Effect_Shadow )
        {
            ShadowEffectSettingsPanel * shadowPanel = new ShadowEffectSettingsPanel(this);
            QObject::connect(shadowPanel, SIGNAL(valueChanged(SFBShadowEffectSettings)), this, SLOT(updateDocumentFromGui()));
            panel = shadowPanel;
        }
        else if ( effectType == SFBEffectType::Effect_Stroke )
        {
            StrokeEffectSettingsPanel * strokePanel = new StrokeEffectSettingsPanel(this);
            QObject::connect(strokePanel, SIGNAL(valueChanged(SFBStrokeEffectSettings)), this, SLOT(updateDocumentFromGui()));
            panel = strokePanel;
        }

        if ( panel != nullptr )
        {
            ui->stackedEffects->addWidget(panel);
            mEffectPanels.push_back(panel);
        }
    }

    updateGuiFromDocument();
    updateAtlasImage();

    mSuppressAtlasUpdate = false;
}


void MainWindow::updateAtlasImage()
{
    SFBSpriteFont spriteFont = mDocument->getSpriteFont();
    QPixmap pixmap = QPixmap::fromImage(spriteFont.textureAtlas);
    int pixelRatio = 1;

    if ( mRetinaEnabled )
    {
        pixelRatio = qApp->devicePixelRatio();
        pixmap.setDevicePixelRatio(pixelRatio);
    }

    mGraphicsScene.clear();
    mGraphicsScene.setSceneRect(-10, -10, (pixmap.width() / pixelRatio) + 20, (pixmap.height() / pixelRatio) + 20);
    mGraphicsScene.addRect(0, 0, (pixmap.width() / pixelRatio), (pixmap.height() / pixelRatio), QPen(QColor(0,0,0,0)), atlasBackgroundBrush());
    mGraphicsScene.addRect(0, 0, (pixmap.width() / pixelRatio), (pixmap.height() / pixelRatio), QPen(QColor(0,0,0,255), 1.0, Qt::DotLine));
    mGraphicsScene.addPixmap(pixmap);

    if ( mRenderGlyphRects )
    {
        for( SFBGlyph & glyph : spriteFont.glyphs )
        {
            QRect rect = glyph.atlasRect;
            rect = QRect((rect.x() / pixelRatio), (rect.y() / pixelRatio), (rect.width() / pixelRatio), (rect.height() / pixelRatio));
            mGraphicsScene.addRect(rect, QPen(QColor(255,0,0,180)), QBrush(QColor(0,0,0,0)));
        }
    }

    ui->atlasGraphicsView->setRenderHint(QPainter::Antialiasing);
    ui->atlasGraphicsView->setRenderHint(QPainter::SmoothPixmapTransform);
    ui->atlasGraphicsView->setTransform(QTransform().scale(mCurrentZoom, mCurrentZoom));

    ui->labelAtlasSize->setText(QString("(%1,%2)").arg(QString::number(spriteFont.textureAtlas.width()), QString::number(spriteFont.textureAtlas.height())));
    ui->labelZoom->setText(QString("%1%").arg(QString::number((int)(mCurrentZoom * 100))));

    if ( !spriteFont.doGlyphsFit ) {
        ui->labelFeedback->setText("Glyphs do not fit on atlas!");
    } else {
        ui->labelFeedback->setText(QString("Generation Time: %1s").arg(QString::number(spriteFont.generationTime, 'f', 3)));
    }

    updateEffectRowPreviews();

    return;
}


void MainWindow::updateEffectRowPreviews()
{
    for( int i = 0; i < mDocument->getEffectCount(); ++i )
    {
        SFBEffect * effect = mDocument->getEffectAtIndex(i).get();

        mEffectListRows[i]->setTitle(effect->getTitle());
        mEffectListRows[i]->setIcon(effect->getPreview(24, 24, 24));
    }
}


void MainWindow::updateGuiFromDocument()
{
    mSuppressAtlasUpdate = true;

    if ( mDocument->getEffectCount() != mEffectPanels.count() ) {
        return;
    }

    ui->inputSettingsPanel->setValue(mDocument->getInputSettings());
    ui->generationSettingsPanel->setValue(mDocument->getGenerationSettings());
    ui->exportSettingsPanel->setValue(mDocument->getExportSettings());

    for( int i = 0; i < mDocument->getEffectCount(); ++i )
    {
        SFBEffect * effect = mDocument->getEffectAtIndex(i).get();
        SFBEffectType effectType = effect->getEffectType();

        mEffectListRows[i]->setChecked(effect->isEnabled());
        mEffectListRows[i]->setTitle(effect->getTitle());
        mEffectListRows[i]->setIcon(effect->getPreview(24, 24, 18));

        if ( effectType == SFBEffectType::Effect_Fill )
        {
            SFBFillEffectSettings settings = dynamic_cast<SFBFillEffect *>(effect)->getSettings();
            FillEffectSettingsPanel * panel = dynamic_cast<FillEffectSettingsPanel *>(mEffectPanels[i]);
            panel->setValue(settings);
        }
        else if ( effectType == SFBEffectType::Effect_Shadow )
        {
            SFBShadowEffectSettings settings = dynamic_cast<SFBShadowEffect *>(effect)->getSettings();
            ShadowEffectSettingsPanel * panel = dynamic_cast<ShadowEffectSettingsPanel *>(mEffectPanels[i]);
            panel->setValue(settings);
        }
        else if ( effectType == SFBEffectType::Effect_Stroke )
        {
            SFBStrokeEffectSettings settings = dynamic_cast<SFBStrokeEffect *>(effect)->getSettings();
            StrokeEffectSettingsPanel * panel = dynamic_cast<StrokeEffectSettingsPanel *>(mEffectPanels[i]);
            panel->setValue(settings);
        }
    }

    mSuppressAtlasUpdate = false;
}


void MainWindow::updateDocumentFromGui()
{
    if ( mSuppressAtlasUpdate ) {
        return;
    }

    if ( mDocument->getEffectCount() != mEffectPanels.count() ) {
        return;
    }

    mDocument->setInputSettings(ui->inputSettingsPanel->getValue());
    mDocument->setGenerationSettings(ui->generationSettingsPanel->getValue());
    mDocument->setExportSettings(ui->exportSettingsPanel->getValue());

    for( int i = 0; i < mDocument->getEffectCount(); ++i )
    {
        SFBEffect * effect = mDocument->getEffectAtIndex(i).get();
        SFBEffectType effectType = effect->getEffectType();

        if ( effectType == SFBEffectType::Effect_Fill )
        {
            FillEffectSettingsPanel * panel = dynamic_cast<FillEffectSettingsPanel *>(mEffectPanels[i]);
            SFBFillEffectSettings settings = panel->getValue();

            settings.enabled = mEffectListRows[i]->isChecked();
            dynamic_cast<SFBFillEffect *>(effect)->setSettings(settings);
        }
        else if ( effectType == SFBEffectType::Effect_Shadow )
        {
            ShadowEffectSettingsPanel * panel = dynamic_cast<ShadowEffectSettingsPanel *>(mEffectPanels[i]);
            SFBShadowEffectSettings settings = panel->getValue();

            settings.enabled = mEffectListRows[i]->isChecked();
            dynamic_cast<SFBShadowEffect *>(effect)->setSettings(settings);
        }
        else if ( effectType == SFBEffectType::Effect_Stroke )
        {
            StrokeEffectSettingsPanel * panel = dynamic_cast<StrokeEffectSettingsPanel *>(mEffectPanels[i]);
            SFBStrokeEffectSettings settings = panel->getValue();

            settings.enabled = mEffectListRows[i]->isChecked();
            dynamic_cast<SFBStrokeEffect *>(effect)->setSettings(settings);
        }
    }

    updateAtlasImage();
}


void MainWindow::selectedEffectChanged()
{
    int index = selectedEffectIndex();
    ui->labelSelectedEffect->setText(mDocument->getEffectAtIndex(index)->getTitle());
    ui->stackedEffects->setCurrentIndex(index);
}


void MainWindow::effectListRowsMoved(const QModelIndex & /*sourceParent*/, int sourceStart, int /*sourceEnd*/, const QModelIndex & /*destinationParent*/, int destinationRow)
{
    int fromIndex = sourceStart;
    int toIndex = destinationRow;
    if ( toIndex > fromIndex ) { toIndex -= 1; }

    //qDebug() << "Moving effect from " << fromIndex << " to " << toIndex;

    SFBEffect::Ptr effect = mDocument->getEffectAtIndex(fromIndex);
    mDocument->removeEffect(fromIndex);
    mDocument->insertEffect(toIndex, effect);

    recreateGuiFromDocument();
}


void MainWindow::effectListRowContextMenu(const QPoint & pos)
{
    int selectedIndex = selectedEffectIndex();

    QMenu menu;
    menu.addAction(ui->actionDelete_Selected_Effect);
    menu.addAction(ui->actionDuplicate_Selected_Effect);

    QWidget * widget = ui->listWidgetEffects->itemWidget(ui->listWidgetEffects->item(selectedIndex));
    menu.exec(widget->mapToGlobal(pos));
}


int MainWindow::selectedEffectIndex()
{
    return ui->listWidgetEffects->currentRow();
}


QBrush MainWindow::atlasBackgroundBrush()
{
    QPixmap pm(10, 10);
    QPainter pmp(&pm);
    pmp.fillRect(0, 0, 5, 5, Qt::white);
    pmp.fillRect(5, 5, 5, 5, Qt::white);
    pmp.fillRect(0, 5, 5, 5, QColor(220,220,220,255));
    pmp.fillRect(5, 0, 5, 5, QColor(220,220,220,255));
    pmp.end();

    return QBrush(pm);
}


void MainWindow::on_actionExport_PNG_triggered()
{
    SFBSpriteFont spriteFont = mDocument->getSpriteFont();

    if ( spriteFont.isNull || spriteFont.textureAtlas.isNull() ) {
        QMessageBox::information(this, "No Sprite Atlas", "No Sprite Atlas available for export", QMessageBox::Ok);
        return;
    }

    QString path = QFileDialog::getSaveFileName(this, "Export PNG");

    if ( !path.isNull() )
    {
        SFBFontExporterPNG exporter;
        exporter.exportFont(mDocument.get());
    }
}


void MainWindow::on_actionToggle_Retina_Support_triggered()
{
    mRetinaEnabled = !mRetinaEnabled;
    updateAtlasImage();
}


void MainWindow::on_toolButtonZoomIn_clicked()
{
    mCurrentZoom += 0.25f;
    updateAtlasImage();
}


void MainWindow::on_toolButtonZoomOut_clicked()
{
    mCurrentZoom -= 0.25f;

    if ( mCurrentZoom < 0.25f ) {
        mCurrentZoom = 0.25f;
    }

    updateAtlasImage();
}


void MainWindow::on_toolButtonZoomReset_clicked()
{
    mCurrentZoom = 1.0f;
    updateAtlasImage();
}


void MainWindow::on_actionSave_triggered()
{
    QString path = mDocument->documentPath();

    if ( path.isEmpty() || !QFileInfo(path).exists() )
    {
        on_actionSaveAs_triggered();
        return;
    }

    SFBFileWriter fileWriter(path);
    fileWriter.writeDocument(mDocument.get());
}

void MainWindow::on_actionOpen_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("Sprite Font Builder File (*.sfb)"));

    if ( !path.isNull() )
    {
        SFBFileReader fileReader;
        SFBDocument::Ptr loadedDocument = fileReader.loadDocument(path);

        if ( loadedDocument != nullptr ) {
            setDocument(loadedDocument);
        }
    }
}

void MainWindow::on_actionNew_triggered()
{
    if ( mDocument->needsSaving() )
    {
        int button = QMessageBox::question(this, "Are you sure?", "Any changes made to the current document will be lost. Are you sure?", "No", "Yes");

        if ( button == 0 ) {
            return;
        }
    }

    loadNewDocument();
}

void MainWindow::on_actionAdd_Fill_triggered()
{
    mDocument->addEffect(SFBEffect::Ptr(new SFBFillEffect()));
    recreateGuiFromDocument();
}

void MainWindow::on_actionAdd_Shadow_triggered()
{
    mDocument->addEffect(SFBEffect::Ptr(new SFBShadowEffect()));
    recreateGuiFromDocument();
}

void MainWindow::on_actionAdd_Stroke_triggered()
{
    mDocument->addEffect(SFBEffect::Ptr(new SFBStrokeEffect()));
    recreateGuiFromDocument();
}

void MainWindow::on_actionDelete_Selected_Effect_triggered()
{
    int effectIndex = selectedEffectIndex();

    if ( effectIndex < 0 ) {
        QMessageBox::warning(this, "Please Select an Effect", "Please select a fill, shadow or stroke effect from the left hand panel by clicking on that effect's title bar.");
        return;
    }

    ui->listWidgetEffects->setCurrentIndex(ui->listWidgetEffects->model()->index(0,0));
    mDocument->removeEffect(effectIndex);
    recreateGuiFromDocument();
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save File"), QString(), tr("Sprite Font Builder File (*.sfb)"));

    if ( !path.isNull() )
    {
        SFBFileWriter fileWriter(path);
        fileWriter.writeDocument(mDocument.get());
    }
}

void MainWindow::on_actionDuplicate_Selected_Effect_triggered()
{
    int effectIndex = selectedEffectIndex();

    if ( effectIndex < 0 ) {
        QMessageBox::warning(this, "Please Select an Effect", "Please select a fill, shadow or stroke effect from the left hand panel by clicking on that effect's title bar.");
        return;
    }

    SFBEffect::Ptr effect = mDocument->getEffectAtIndex(effectIndex);
    SFBEffect::Ptr effectCopy = effect->clone();
    mDocument->insertEffect(effectIndex+1, effectCopy);
    recreateGuiFromDocument();
}

void MainWindow::on_actionExport_triggered()
{
    SFBExportSettings exportSettings = mDocument->getExportSettings();

    if ( exportSettings.path.isNull() ) {
        QMessageBox::information(this, tr("Set Export Path"), tr("You must set an export path in the 'Export Settings Panel' before attempting to export a file."), QMessageBox::Ok);
        return;
    }

    SFBSpriteFont spriteFont = mDocument->getSpriteFont();

    if ( spriteFont.isNull || spriteFont.textureAtlas.isNull() ) {
        QMessageBox::information(this, tr("No Sprite Atlas"), tr("No Sprite Atlas available for export"), QMessageBox::Ok);
        return;
    }

    SFBFontExporter::Ptr exporter = SFBFontExporter::FontExporterForSettings(exportSettings);

    if ( exporter == nullptr ) {
        QMessageBox::information(this, tr("No Valid Exporter"), tr("Could not find valid exporter for this file type. Sorry!"), QMessageBox::Ok);
        return;
    }

    exporter->exportFont(mDocument.get());
}

void MainWindow::on_actionToggle_Glyph_Outlines_triggered()
{
    mRenderGlyphRects = !mRenderGlyphRects;
    updateAtlasImage();
}
