/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// Qt includes
#include <QDebug>

// QtGUI includes
#include "qSlicerApplication.h"
#include "qSlicerMarkupsSettingsPanel.h"
#include "ui_qSlicerMarkupsSettingsPanel.h"

// Markups Logic includes
#include <vtkSlicerMarkupsLogic.h>

// Markups MRML includes

// VTK includes
#include <vtkNew.h>
#include <vtkSmartPointer.h>
#include <vtkMRMLScene.h>
#include <vtkMRMLMarkupsNode.h>
#include <vtkMRMLDisplayNode.h>
#include <vtkMRMLMarkupsDisplayNode.h>
#include <vtkMRMLMarkupsCurveNode.h>
#include <qsettings.h>
// --------------------------------------------------------------------------
// qSlicerMarkupsSettingsPanelPrivate

//-----------------------------------------------------------------------------
class qSlicerMarkupsSettingsPanelPrivate: public Ui_qSlicerMarkupsSettingsPanel
{
  Q_DECLARE_PUBLIC(qSlicerMarkupsSettingsPanel);
protected:
  qSlicerMarkupsSettingsPanel* const q_ptr;

public:
  qSlicerMarkupsSettingsPanelPrivate(qSlicerMarkupsSettingsPanel& object);
  void init();

  vtkSmartPointer<vtkSlicerMarkupsLogic> MarkupsLogic;
};

// --------------------------------------------------------------------------
// qSlicerMarkupsSettingsPanelPrivate methods

// --------------------------------------------------------------------------
qSlicerMarkupsSettingsPanelPrivate
::qSlicerMarkupsSettingsPanelPrivate(qSlicerMarkupsSettingsPanel& object)
  :q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qSlicerMarkupsSettingsPanelPrivate::init()
{
  Q_Q(qSlicerMarkupsSettingsPanel);

  this->setupUi(q);
}



// --------------------------------------------------------------------------
// qSlicerMarkupsSettingsPanel methods

// --------------------------------------------------------------------------
qSlicerMarkupsSettingsPanel::qSlicerMarkupsSettingsPanel(QWidget* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerMarkupsSettingsPanelPrivate(*this))
{
  Q_D(qSlicerMarkupsSettingsPanel);
  d->init();
}

// --------------------------------------------------------------------------
qSlicerMarkupsSettingsPanel::~qSlicerMarkupsSettingsPanel() = default;

// --------------------------------------------------------------------------
vtkSlicerMarkupsLogic* qSlicerMarkupsSettingsPanel
::markupsLogic()const
{
  Q_D(const qSlicerMarkupsSettingsPanel);
  return d->MarkupsLogic;
}

// --------------------------------------------------------------------------
void qSlicerMarkupsSettingsPanel
::setMarkupsLogic(vtkSlicerMarkupsLogic* logic)
{
  Q_D(qSlicerMarkupsSettingsPanel);

  qvtkReconnect(d->MarkupsLogic, logic, vtkCommand::ModifiedEvent,
                this, SLOT(onMarkupsLogicModified()));
  d->MarkupsLogic = logic;
   
  this->onMarkupsLogicModified();

  this->registerProperty("Markups/GlyphType", this,
                         "defaultGlyphType", SIGNAL(defaultGlyphTypeChanged(QString)));
  this->registerProperty("Markups/SelectedColor", this,
                         "defaultSelectedColor", SIGNAL(defaultSelectedColorChanged(QColor)));
  this->registerProperty("Markups/UnselectedColor", this,
                         "defaultUnselectedColor", SIGNAL(defaultUnselectedColorChanged(QColor)));
  this->registerProperty("Markups/GlyphScale", this,
                         "defaultGlyphScale", SIGNAL(defaultGlyphScaleChanged(double)));
  this->registerProperty("Markups/TextScale", this,
                         "defaultTextScale", SIGNAL(defaultTextScaleChanged(double)));
  this->registerProperty("Markups/Opacity", this,
                         "defaultOpacity", SIGNAL(defaultOpacityChanged(double)));
}

// --------------------------------------------------------------------------
void qSlicerMarkupsSettingsPanel
::onMarkupsLogicModified()
{
  Q_D(qSlicerMarkupsSettingsPanel);
/* disable it for now; if we want a settings panel then use the same pattern that is used for default view options

  // update the gui to match the logic
  QString glyphType = QString(d->MarkupsLogic->GetDefaultMarkupsDisplayNodeGlyphTypeAsString().c_str());

  

  // TODO: do I need to use the strings?
//  d->defaultGlyphTypeComboBox->setCurrentIndex(glyphType - 1);
  int glyphTypeIndex = d->defaultGlyphTypeComboBox->findData(glyphType);
  if (glyphTypeIndex != -1)
    {
    d->defaultGlyphTypeComboBox->setCurrentIndex(glyphTypeIndex);
    }


  double *unselectedColor = d->MarkupsLogic->GetDefaultMarkupsDisplayNodeColor();
  
  QColor qcolor = QColor::fromRgbF(unselectedColor[0], unselectedColor[1], unselectedColor[2]);
  d->defaultUnselectedColorPickerButton->setColor(qcolor);

  double *selectedColor =  d->MarkupsLogic->GetDefaultMarkupsDisplayNodeSelectedColor();
  
  qcolor = QColor::fromRgbF(selectedColor[0], selectedColor[1], selectedColor[2]);
  d->defaultSelectedColorPickerButton->setColor(qcolor);

  double glyphScale = d->MarkupsLogic->GetDefaultMarkupsDisplayNodeGlyphScale();
  d->defaultGlyphScaleSliderWidget->setValue(glyphScale);

  
  d->defaultTextScaleSliderWidget->setValue(textScale);

  double opacity = d->MarkupsLogic->GetDefaultMarkupsDisplayNodeOpacity();
  
  d->defaultOpacitySliderWidget->setValue(opacity);
  */
  QObject::connect(d->defaultOpacitySliderWidget, SIGNAL(valueChanged(double)),
      this, SLOT(onDefaultOpacityChanged(double)), Qt::UniqueConnection);
  QObject::connect(d->defaultSelectedColorPickerButton, SIGNAL(colorChanged(QColor)),
      this, SLOT(onDefaultSelectedColorChanged(QColor)), Qt::UniqueConnection);
  QObject::connect(d->defaultTextScaleSliderWidget, SIGNAL(valueChanged(double)),
      this, SLOT(onDefaultTextScaleChanged(double)), Qt::UniqueConnection);
  QObject::connect(d->defaultGlyphScaleSliderWidget, SIGNAL(valueChanged(double)),
      this, SLOT(onDefaultGlyphScaleChanged(double)), Qt::UniqueConnection);
  QObject::connect(d->defaultGlyphTypeComboBox, SIGNAL(currentIndexChanged(int)),
      this, SLOT(onDefaultGlyphTypeChanged(int)), Qt::UniqueConnection);
  QObject::connect(d->defaultUnselectedColorPickerButton, SIGNAL(colorChanged(QColor)),
      this, SLOT(onDefaultUnselectedColorChanged(QColor)));
  //QObject::connect(d->defaultCurveTypeComboBox, SIGNAL(currentIndexChanged(int)),
    //  this, SLOT(onDefaultCurveTypeChanged(int)), Qt::UniqueConnection);
}

// --------------------------------------------------------------------------
QString qSlicerMarkupsSettingsPanel::defaultGlyphType()const
{
  Q_D(const qSlicerMarkupsSettingsPanel);

  int currentIndex  = d->defaultGlyphTypeComboBox->currentIndex();
  QString glyphType;
  if (currentIndex != -1)
    {
    glyphType =
      d->defaultGlyphTypeComboBox->itemText(currentIndex);
    }
  return glyphType;
}

// --------------------------------------------------------------------------
QColor qSlicerMarkupsSettingsPanel::defaultUnselectedColor()const
{
  Q_D(const qSlicerMarkupsSettingsPanel);

  QColor color = d->defaultUnselectedColorPickerButton->color();

  return color;
}

// --------------------------------------------------------------------------
QColor qSlicerMarkupsSettingsPanel::defaultSelectedColor()const
{
  Q_D(const qSlicerMarkupsSettingsPanel);

  QColor color = d->defaultSelectedColorPickerButton->color();

  return color;
}



// --------------------------------------------------------------------------
double qSlicerMarkupsSettingsPanel::defaultGlyphScale()const
{
  Q_D(const qSlicerMarkupsSettingsPanel);

  double glyphScale = d->defaultGlyphScaleSliderWidget->value();

  return glyphScale;
}

// --------------------------------------------------------------------------
double qSlicerMarkupsSettingsPanel::defaultTextScale()const
{
  Q_D(const qSlicerMarkupsSettingsPanel);

  double textScale = d->defaultTextScaleSliderWidget->value();

  return textScale;
}

// --------------------------------------------------------------------------
double qSlicerMarkupsSettingsPanel::defaultOpacity()const
{
  Q_D(const qSlicerMarkupsSettingsPanel);

  double opacity = d->defaultOpacitySliderWidget->value();

  return opacity;
}

// --------------------------------------------------------------------------
void qSlicerMarkupsSettingsPanel::setDefaultGlyphType(const QString& glyphType)
{
  Q_D(qSlicerMarkupsSettingsPanel);

  int glyphTypeIndex = d->defaultGlyphTypeComboBox->findData(glyphType);

  if (glyphTypeIndex != -1)
    {
    d->defaultGlyphTypeComboBox->setCurrentIndex(glyphTypeIndex);
    }
}

// --------------------------------------------------------------------------
void qSlicerMarkupsSettingsPanel::setDefaultUnselectedColor(const QColor color)
{
  Q_D(qSlicerMarkupsSettingsPanel);

  d->defaultUnselectedColorPickerButton->setColor(color);
}

// --------------------------------------------------------------------------
void qSlicerMarkupsSettingsPanel::setDefaultSelectedColor(const QColor color)
{
  Q_D(qSlicerMarkupsSettingsPanel);

  d->defaultSelectedColorPickerButton->setColor(color);
}


// --------------------------------------------------------------------------
void qSlicerMarkupsSettingsPanel::setDefaultGlyphScale(const double glyphScale)
{
  Q_D(qSlicerMarkupsSettingsPanel);

  d->defaultGlyphScaleSliderWidget->setValue(glyphScale);
}

// --------------------------------------------------------------------------
void qSlicerMarkupsSettingsPanel::setDefaultTextScale(const double glyphScale)
{
  Q_D(qSlicerMarkupsSettingsPanel);

  d->defaultTextScaleSliderWidget->setValue(glyphScale);
}

// --------------------------------------------------------------------------
void qSlicerMarkupsSettingsPanel::setDefaultOpacity(const double opacity)
{
  Q_D(qSlicerMarkupsSettingsPanel);

  d->defaultOpacitySliderWidget->setValue(opacity);
}

// --------------------------------------------------------------------------
void qSlicerMarkupsSettingsPanel::onDefaultGlyphTypeChanged(int index)
{
    Q_D(qSlicerMarkupsSettingsPanel);
  auto scene = d->MarkupsLogic->GetMRMLScene();
  std::vector<vtkMRMLNode*> mNodes;
  int nnodes = scene ? scene->GetNodesByClass("vtkMRMLMarkupsNode", mNodes) : 0;
  for (int i = 0; i < nnodes; i++)
  {
      auto mNode = vtkMRMLMarkupsNode::SafeDownCast(mNodes[i]);
      if (!mNode) {
          continue;
      }
      auto displayNode = vtkMRMLMarkupsDisplayNode::SafeDownCast(mNode->GetDisplayNode());
      if (!displayNode) {
          continue;
      }
      displayNode->SetGlyphType(index);
  }
  QSettings settings;
  settings.setValue("Markups/GlyphType", d->defaultGlyphTypeComboBox->currentText());
  auto dn = d->MarkupsLogic->GetDefaultMarkupsDisplayNode();
  if (dn) {
      dn->SetGlyphType(index);
  }
  emit defaultGlyphTypeChanged(d->defaultGlyphTypeComboBox->currentText());
}

// --------------------------------------------------------------------------
void qSlicerMarkupsSettingsPanel::updateMarkupsLogicDefaultGlyphType()
{
  Q_D(qSlicerMarkupsSettingsPanel);

  if (d->MarkupsLogic == nullptr)
    {
    return;
    }
  // disable it for now; if we want a settings panel then use the same pattern that is used for default view options
  // d->MarkupsLogic->SetDefaultMarkupsDisplayNodeGlyphTypeFromString(this->defaultGlyphType().toUtf8());
}



// --------------------------------------------------------------------------
void qSlicerMarkupsSettingsPanel::onDefaultSelectedColorChanged(QColor qcolor)
{
    Q_D(qSlicerMarkupsSettingsPanel);

    double color[3];

    color[0] = qcolor.redF();
    color[1] = qcolor.greenF();
    color[2] = qcolor.blueF();

    // disable it for now; if we want a settings panel then use the same pattern that is used for default view options
    auto scene = d->MarkupsLogic->GetMRMLScene();
    std::vector<vtkMRMLNode*> mNodes;
    int nnodes = scene ? scene->GetNodesByClass("vtkMRMLMarkupsNode", mNodes) : 0;
    for (int i = 0; i < nnodes; i++)
    {
        auto mNode = vtkMRMLMarkupsNode::SafeDownCast(mNodes[i]);
        if (!mNode) {
            continue;
        }
        auto displayNode = vtkMRMLMarkupsDisplayNode::SafeDownCast(mNode->GetDisplayNode());
        if (!displayNode) {
            continue;
        }
        displayNode->SetSelectedColor(color);
    }
    QSettings settings;
    settings.setValue("Markups/SelectedColor", qcolor);
    auto dn = d->MarkupsLogic->GetDefaultMarkupsDisplayNode();
    if (dn) {
        dn->SetSelectedColor(color);
    }
    emit(defaultSelectedColorChanged(qcolor));
}

// --------------------------------------------------------------------------
void qSlicerMarkupsSettingsPanel::updateMarkupsLogicDefaultUnselectedColor()
{
  Q_D(qSlicerMarkupsSettingsPanel);

  QColor qcolor = this->defaultUnselectedColor();

  double color[3];

  color[0] = qcolor.redF();
  color[1] = qcolor.greenF();
  color[2] = qcolor.blueF();

  // disable it for now; if we want a settings panel then use the same pattern that is used for default view options
  // d->MarkupsLogic->SetDefaultMarkupsDisplayNodeColor(color);
  Q_UNUSED(color);
}


void qSlicerMarkupsSettingsPanel::onDefaultCurveTypeChanged(int index) {
    Q_D(qSlicerMarkupsSettingsPanel);
    auto scene = d->MarkupsLogic->GetMRMLScene();
    std::vector<vtkMRMLNode*> mNodes;
    int nnodes = scene ? scene->GetNodesByClass("vtkMRMLMarkupsNode", mNodes) : 0;
    for (int i = 0; i < nnodes; i++)
    {
        auto mNode = vtkMRMLMarkupsCurveNode::SafeDownCast(mNodes[i]);
        if (!mNode) {
            continue;
        }
        mNode->SetCurveType(index);
    }
    QSettings settings;
    settings.setValue("Markups/CurveType", index);
}

// --------------------------------------------------------------------------
void qSlicerMarkupsSettingsPanel::onDefaultUnselectedColorChanged(QColor qcolor)
{
    Q_D(qSlicerMarkupsSettingsPanel);

    double color[3];

    color[0] = qcolor.redF();
    color[1] = qcolor.greenF();
    color[2] = qcolor.blueF();

    // disable it for now; if we want a settings panel then use the same pattern that is used for default view options
    auto scene = d->MarkupsLogic->GetMRMLScene();
    std::vector<vtkMRMLNode*> mNodes;
    int nnodes = scene ? scene->GetNodesByClass("vtkMRMLMarkupsNode", mNodes) : 0;
    for (int i = 0; i < nnodes; i++)
    {
        auto mNode = vtkMRMLMarkupsNode::SafeDownCast(mNodes[i]);
        if (!mNode) {
            continue;
        }
        auto displayNode = vtkMRMLMarkupsDisplayNode::SafeDownCast(mNode->GetDisplayNode());
        if (!displayNode) {
            continue;
        }
        displayNode->SetActiveColor(color);
    }
    QSettings settings;
    settings.setValue("Markups/UnselectedColor", qcolor);
    auto dn = d->MarkupsLogic->GetDefaultMarkupsDisplayNode();
    if (dn) {
        dn->SetActiveColor(color);
    }
    emit(defaultUnselectedColorChanged(qcolor));
}

// --------------------------------------------------------------------------
void qSlicerMarkupsSettingsPanel::updateMarkupsLogicDefaultSelectedColor()
{
  Q_D(qSlicerMarkupsSettingsPanel);

}

// --------------------------------------------------------------------------
void qSlicerMarkupsSettingsPanel::onDefaultGlyphScaleChanged(double scale)
{
  
  Q_D(qSlicerMarkupsSettingsPanel);

  auto scene = d->MarkupsLogic->GetMRMLScene();
  std::vector<vtkMRMLNode*> mNodes;
  int nnodes = scene ? scene->GetNodesByClass("vtkMRMLMarkupsNode", mNodes) : 0;
  for (int i = 0; i < nnodes; i++)
  {
      auto mNode = vtkMRMLMarkupsNode::SafeDownCast(mNodes[i]);
      if (!mNode) {
          continue;
      }
      auto displayNode = vtkMRMLMarkupsDisplayNode::SafeDownCast(mNode->GetDisplayNode());
      if (!displayNode) {
          continue;
      }
      displayNode->SetGlyphScale(scale);
  }
  QSettings settings;
  settings.setValue("Markups/GlyphScale", scale);
  auto dn = d->MarkupsLogic->GetDefaultMarkupsDisplayNode();
  if (dn) {
      dn->SetGlyphScale(scale);
  }
  emit defaultGlyphScaleChanged(this->defaultGlyphScale());

}

// --------------------------------------------------------------------------
void qSlicerMarkupsSettingsPanel::updateMarkupsLogicDefaultGlyphScale()
{
  Q_D(qSlicerMarkupsSettingsPanel);

  if (d->MarkupsLogic == nullptr)
    {
    return;
    }
  // disable it for now; if we want a settings panel then use the same pattern that is used for default view options
  // d->MarkupsLogic->SetDefaultMarkupsDisplayNodeGlyphScale(this->defaultGlyphScale());
}

// --------------------------------------------------------------------------
void qSlicerMarkupsSettingsPanel::onDefaultTextScaleChanged(double scale)
{
  Q_D(qSlicerMarkupsSettingsPanel);
  this->updateMarkupsLogicDefaultTextScale();
  emit defaultTextScaleChanged(scale);

  auto scene = d->MarkupsLogic->GetMRMLScene();
  std::vector<vtkMRMLNode*> mNodes;
  int nnodes = scene ? scene->GetNodesByClass("vtkMRMLMarkupsNode", mNodes) : 0;
  for (int i = 0; i < nnodes; i++)
  {
      auto mNode = vtkMRMLMarkupsNode::SafeDownCast(mNodes[i]);
      if (!mNode) {
          continue;
      }
      auto displayNode = vtkMRMLMarkupsDisplayNode::SafeDownCast(mNode->GetDisplayNode());
      if (!displayNode) {
          continue;
      }
      displayNode->SetTextScale(scale);
  }
  QSettings settings;
  settings.setValue("Markups/TextScale", scale);
  auto dn = d->MarkupsLogic->GetDefaultMarkupsDisplayNode();
  if (dn) {
      dn->SetTextScale(scale);
  }
}

// --------------------------------------------------------------------------
void qSlicerMarkupsSettingsPanel::updateMarkupsLogicDefaultTextScale()
{
  Q_D(qSlicerMarkupsSettingsPanel);

  if (d->MarkupsLogic == nullptr)
    {
    return;
    }
  // disable it for now; if we want a settings panel then use the same pattern that is used for default view options
  // d->MarkupsLogic->SetDefaultMarkupsDisplayNodeTextScale(this->defaultTextScale());
}

// --------------------------------------------------------------------------
void qSlicerMarkupsSettingsPanel::onDefaultOpacityChanged(double opacity)
{
    Q_D(qSlicerMarkupsSettingsPanel);
  
  auto scene = d->MarkupsLogic->GetMRMLScene();
  std::vector<vtkMRMLNode*> mNodes;
  int nnodes = scene ? scene->GetNodesByClass("vtkMRMLMarkupsNode", mNodes) : 0;
  for (int i = 0; i < nnodes; i++)
  {
      auto mNode = vtkMRMLMarkupsNode::SafeDownCast(mNodes[i]);
      if (!mNode) {
          continue;
      }
      auto displayNode = mNode->GetDisplayNode();
      if (!displayNode) {
          continue;
      }
      displayNode->SetOpacity(opacity);
  }
  QSettings settings;
  settings.setValue("Markups/Opacity", opacity);
  auto dn = d->MarkupsLogic->GetDefaultMarkupsDisplayNode();
  if(dn){
      dn->SetOpacity(opacity);
  }
  emit defaultOpacityChanged(opacity);
}

// --------------------------------------------------------------------------
void qSlicerMarkupsSettingsPanel::updateMarkupsLogicDefaultOpacity()
{
  Q_D(qSlicerMarkupsSettingsPanel);

  if (d->MarkupsLogic == nullptr)
    {
    return;
    }
  // disable it for now; if we want a settings panel then use the same pattern that is used for default view options
  // d->MarkupsLogic->SetDefaultMarkupsDisplayNodeOpacity(this->defaultOpacity());
}
