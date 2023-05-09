/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Csaba Pinter, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

// SubjectHierarchy MRML includes
#include "vtkMRMLSubjectHierarchyNode.h"
#include "vtkMRMLSubjectHierarchyConstants.h"

// SubjectHierarchy Plugins includes
#include "qSlicerSubjectHierarchyPluginHandler.h"
#include "qSlicerSubjectHierarchyMarkupsPlugin.h"
#include "qSlicerSubjectHierarchyDefaultPlugin.h"

// Terminologies includes
#include "qSlicerTerminologyItemDelegate.h"
#include "vtkSlicerTerminologiesModuleLogic.h"

// MRML widgets includes
#include "qMRMLNodeComboBox.h"

// MRML includes
#include <vtkMRMLDisplayableNode.h>
#include <vtkMRMLDisplayNode.h>
#include <vtkMRMLMarkupsDisplayNode.h>
#include <vtkMRMLMarkupsROIDisplayNode.h>
#include <vtkMRMLMarkupsNode.h>
#include <vtkMRMLScene.h>

// vtkSegmentationCore includes
#include <vtkSegment.h>

// VTK includes
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>

// Qt includes
#include <QAction>
#include <QDebug>
#include <QInputDialog>
#include <QMenu>
#include <QStandardItem>

// Slicer includes
#include "qSlicerAbstractModuleWidget.h"
#include "vtkMRMLMarkupsLineNode.h"
#include "vtkMatrix4x4.h"
#include "vtkTransform.h"
#include "qSlicerApplication.h"
#include "qMRMLViewControllerBar.h"
#include "qSlicerLayoutManager.h"
#include "vtkMRMLSliceNode.h"
#include "qMRMLSliceWidget.h"
#include "qMRMLSliceControllerWidget.h"
#include "QTimer.h"
#include "vtkMRMLMarkupsCurveNode.h"
//-----------------------------------------------------------------------------
const char* INTERACTION_HANDLE_TYPE_PROPERTY = "InteractionHandleType";

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_SubjectHierarchy_Plugins
class qSlicerSubjectHierarchyMarkupsPluginPrivate: public QObject
{
  Q_DECLARE_PUBLIC(qSlicerSubjectHierarchyMarkupsPlugin);
protected:
  qSlicerSubjectHierarchyMarkupsPlugin* const q_ptr;
public:
  qSlicerSubjectHierarchyMarkupsPluginPrivate(qSlicerSubjectHierarchyMarkupsPlugin& object);
  ~qSlicerSubjectHierarchyMarkupsPluginPrivate() override;
  void init();

public:
  QColorDialog* m_pColorDialog;
  QAction* RenamePointAction;
  QAction* DeletePointAction;
  QAction* ToggleSelectPointAction;
  QAction* ToggleCurrentItemHandleInteractive;
  QAction* ToggleHandleInteractive;
  QAction* HideLabelInteractive;
  QAction* ChangeColorInteractive;
  QAction* ChangeCameraDirectionInteractive;
  QAction* CommonSettingInteractive;
  QAction* SliceProjectionInteractive;
  QAction* ResampleInteractive;

  QMenu* CurrentItemHandleVisibilityMenu;
  QAction* CurrentItemHandleVisibilityAction;
  QAction* ToggleCurrentItemTranslateHandleVisible;
  QAction* ToggleCurrentItemRotateHandleVisible;
  QAction* ToggleCurrentItemScaleHandleVisible;

  QMenu* HandleVisibilityMenu;
  QAction* HandleVisibilityAction;
  QAction* ToggleTranslateHandleVisible;
  QAction* ToggleRotateHandleVisible;
  QAction* ToggleScaleHandleVisible;

  QVariantMap ViewMenuEventData;
};

//-----------------------------------------------------------------------------
// qSlicerSubjectHierarchyMarkupsPluginPrivate methods

//-----------------------------------------------------------------------------
qSlicerSubjectHierarchyMarkupsPluginPrivate::qSlicerSubjectHierarchyMarkupsPluginPrivate(qSlicerSubjectHierarchyMarkupsPlugin& object)
: q_ptr(&object)
, RenamePointAction(nullptr)
, DeletePointAction(nullptr)
, ToggleSelectPointAction(nullptr)
, ToggleCurrentItemHandleInteractive(nullptr)
, ToggleHandleInteractive(nullptr)
, HideLabelInteractive(nullptr)
, ChangeColorInteractive(nullptr)
, ChangeCameraDirectionInteractive(nullptr)
, SliceProjectionInteractive(nullptr)
, ResampleInteractive(nullptr)
, CommonSettingInteractive(nullptr)
, m_pColorDialog(nullptr)
{
}

//------------------------------------------------------------------------------
void qSlicerSubjectHierarchyMarkupsPluginPrivate::init()
{
  Q_Q(qSlicerSubjectHierarchyMarkupsPlugin);

  this->RenamePointAction = new QAction("Rename point...", q);
  this->RenamePointAction->setObjectName("RenamePointAction");
  QObject::connect(this->RenamePointAction, SIGNAL(triggered()), q, SLOT(renamePoint()));

  this->DeletePointAction = new QAction(QStringLiteral("删除形状"), q);
  this->DeletePointAction->setObjectName("DeletePointAction");
  QObject::connect(this->DeletePointAction, SIGNAL(triggered()), q, SLOT(deletePoint()));

  this->ToggleSelectPointAction = new QAction("Toggle select point", q);
  this->ToggleSelectPointAction->setObjectName("ToggleSelectPointAction");
  QObject::connect(this->ToggleSelectPointAction, SIGNAL(triggered()), q, SLOT(toggleSelectPoint()));

  this->ToggleCurrentItemHandleInteractive = new QAction("Interaction");
  this->ToggleCurrentItemHandleInteractive->setObjectName("ToggleCurrentItemHandleInteractive");
  this->ToggleCurrentItemHandleInteractive->setCheckable(true);
  QObject::connect(this->ToggleCurrentItemHandleInteractive, SIGNAL(triggered()), q, SLOT(toggleCurrentItemHandleInteractive()));

  this->ToggleHandleInteractive = new QAction(QStringLiteral("平移旋转"));
  this->ToggleHandleInteractive->setObjectName("ToggleHandleInteractive");
  this->ToggleHandleInteractive->setCheckable(true);
  QObject::connect(this->ToggleHandleInteractive, SIGNAL(triggered()), q, SLOT(toggleHandleInteractive()));

  this->HideLabelInteractive = new QAction(QStringLiteral("隐藏标签"));
  this->HideLabelInteractive->setObjectName("HideLabelInteractive");
  QObject::connect(this->HideLabelInteractive, SIGNAL(triggered()), q, SLOT(toggleHideLabelInteractive()));

  this->ChangeColorInteractive = new QAction(QStringLiteral("更改颜色")); 
  this->ChangeColorInteractive->setObjectName("ChangeColorInteractive");
  QObject::connect(this->ChangeColorInteractive, SIGNAL(triggered()), q, SLOT(toggleChangeColorInteractive()));

  this->ChangeCameraDirectionInteractive = new QAction(QStringLiteral("摄像机适配"));
  this->ChangeCameraDirectionInteractive->setObjectName("ChangeCameraDirectionInteractive");
  QObject::connect(this->ChangeCameraDirectionInteractive, SIGNAL(triggered()), q, SLOT(toggleChangeCameraDirectionInteractive()));

  this->SliceProjectionInteractive = new QAction(QStringLiteral("显示投影"));
  this->SliceProjectionInteractive->setObjectName("SliceProjectionInteractive");
  QObject::connect(this->SliceProjectionInteractive, SIGNAL(triggered()), q, SLOT(toggleSliceProjectionInteractive()));

  this->ResampleInteractive = new QAction(QStringLiteral("点采样"));
  this->ResampleInteractive->setObjectName("ResampleInteractive");
  QObject::connect(this->ResampleInteractive, SIGNAL(triggered()), q, SLOT(toggleResampleInteractive()));


  this->CommonSettingInteractive = new QAction(QStringLiteral("通用配置"));
  this->CommonSettingInteractive->setObjectName("CommonSettingInteractive");
  QObject::connect(this->CommonSettingInteractive, SIGNAL(triggered()), q, SLOT(toggleCommonSettingInteractive()));

  
  this->ToggleCurrentItemTranslateHandleVisible = new QAction("Translate");
  this->ToggleCurrentItemTranslateHandleVisible->setCheckable(true);
  this->ToggleCurrentItemTranslateHandleVisible->setProperty(INTERACTION_HANDLE_TYPE_PROPERTY, vtkMRMLMarkupsDisplayNode::ComponentTranslationHandle);
  QObject::connect(this->ToggleCurrentItemTranslateHandleVisible, SIGNAL(triggered()), q, SLOT(toggleCurrentItemHandleTypeVisibility()));

  this->ToggleCurrentItemRotateHandleVisible = new QAction("Rotate");
  this->ToggleCurrentItemRotateHandleVisible->setCheckable(true);
  this->ToggleCurrentItemRotateHandleVisible->setProperty(INTERACTION_HANDLE_TYPE_PROPERTY, vtkMRMLMarkupsDisplayNode::ComponentRotationHandle);
  QObject::connect(this->ToggleCurrentItemRotateHandleVisible, SIGNAL(triggered()), q, SLOT(toggleCurrentItemHandleTypeVisibility()));

  this->ToggleCurrentItemScaleHandleVisible = new QAction("Scale");
  this->ToggleCurrentItemScaleHandleVisible->setCheckable(true);
  this->ToggleCurrentItemScaleHandleVisible->setProperty(INTERACTION_HANDLE_TYPE_PROPERTY, vtkMRMLMarkupsDisplayNode::ComponentScaleHandle);
  QObject::connect(this->ToggleCurrentItemScaleHandleVisible, SIGNAL(triggered()), q, SLOT(toggleCurrentItemHandleTypeVisibility()));

  this->CurrentItemHandleVisibilityMenu = new QMenu();

  this->CurrentItemHandleVisibilityMenu->addAction(this->ToggleCurrentItemTranslateHandleVisible);
  this->CurrentItemHandleVisibilityMenu->addAction(this->ToggleCurrentItemRotateHandleVisible);
  this->CurrentItemHandleVisibilityMenu->addAction(this->ToggleCurrentItemScaleHandleVisible);

  this->CurrentItemHandleVisibilityAction = new QAction("Interaction options");
  this->CurrentItemHandleVisibilityAction->setMenu(this->CurrentItemHandleVisibilityMenu);

  this->ToggleTranslateHandleVisible = new QAction("Translate");
  this->ToggleTranslateHandleVisible->setCheckable(true);
  this->ToggleTranslateHandleVisible->setProperty(INTERACTION_HANDLE_TYPE_PROPERTY, vtkMRMLMarkupsDisplayNode::ComponentTranslationHandle);
  QObject::connect(this->ToggleTranslateHandleVisible, SIGNAL(triggered()), q, SLOT(toggleHandleTypeVisibility()));

  this->ToggleRotateHandleVisible = new QAction("Rotate");
  this->ToggleRotateHandleVisible->setCheckable(true);
  this->ToggleRotateHandleVisible->setProperty(INTERACTION_HANDLE_TYPE_PROPERTY, vtkMRMLMarkupsDisplayNode::ComponentRotationHandle);
  QObject::connect(this->ToggleRotateHandleVisible, SIGNAL(triggered()), q, SLOT(toggleHandleTypeVisibility()));

  this->ToggleScaleHandleVisible = new QAction("Scale");
  this->ToggleScaleHandleVisible->setCheckable(true);
  this->ToggleScaleHandleVisible->setProperty(INTERACTION_HANDLE_TYPE_PROPERTY, vtkMRMLMarkupsDisplayNode::ComponentScaleHandle);
  QObject::connect(this->ToggleScaleHandleVisible, SIGNAL(triggered()), q, SLOT(toggleHandleTypeVisibility()));

  this->HandleVisibilityMenu = new QMenu();
  this->HandleVisibilityMenu->addAction(this->ToggleTranslateHandleVisible);
  this->HandleVisibilityMenu->addAction(this->ToggleRotateHandleVisible);
  this->HandleVisibilityMenu->addAction(this->ToggleScaleHandleVisible);

  this->HandleVisibilityAction = new QAction("Interaction options");
  this->HandleVisibilityAction->setMenu(this->HandleVisibilityMenu);
}

//-----------------------------------------------------------------------------
qSlicerSubjectHierarchyMarkupsPluginPrivate::~qSlicerSubjectHierarchyMarkupsPluginPrivate() = default;

//-----------------------------------------------------------------------------
// qSlicerSubjectHierarchyMarkupsPlugin methods

//-----------------------------------------------------------------------------
qSlicerSubjectHierarchyMarkupsPlugin::qSlicerSubjectHierarchyMarkupsPlugin(QObject* parent)
 : Superclass(parent)
 , d_ptr( new qSlicerSubjectHierarchyMarkupsPluginPrivate(*this) )
{
  this->m_Name = QString("Markups");

  Q_D(qSlicerSubjectHierarchyMarkupsPlugin);
  d->init();
}

//-----------------------------------------------------------------------------
qSlicerSubjectHierarchyMarkupsPlugin::~qSlicerSubjectHierarchyMarkupsPlugin() = default;

//----------------------------------------------------------------------------
double qSlicerSubjectHierarchyMarkupsPlugin::canAddNodeToSubjectHierarchy(
  vtkMRMLNode* node, vtkIdType parentItemID/*=vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID*/)const
{
  Q_UNUSED(parentItemID);
  if (!node)
    {
    qCritical() << Q_FUNC_INFO << ": Input node is NULL";
    return 0.0;
    }
  else if (node->IsA("vtkMRMLMarkupsFiducialNode") ||
           node->IsA("vtkMRMLMarkupsLineNode") ||
           node->IsA("vtkMRMLMarkupsAngleNode") ||
           node->IsA("vtkMRMLMarkupsCurveNode") ||
           node->IsA("vtkMRMLMarkupsClosedCurveNode") ||
           node->IsA("vtkMRMLMarkupsPlaneNode") ||
           node->IsA("vtkMRMLMarkupsROINode")
           )
    {
    // Item is a markup
    return 0.5;
    }
  return 0.0;
}

//---------------------------------------------------------------------------
double qSlicerSubjectHierarchyMarkupsPlugin::canOwnSubjectHierarchyItem(vtkIdType itemID)const
{
  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return 0.0;
    }
  vtkMRMLSubjectHierarchyNode* shNode = qSlicerSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return 0.0;
    }

  // Markup
  vtkMRMLNode* associatedNode = shNode->GetItemDataNode(itemID);
  if (associatedNode &&
      (associatedNode->IsA("vtkMRMLMarkupsFiducialNode") ||
       associatedNode->IsA("vtkMRMLMarkupsLineNode") ||
       associatedNode->IsA("vtkMRMLMarkupsAngleNode") ||
       associatedNode->IsA("vtkMRMLMarkupsCurveNode") ||
       associatedNode->IsA("vtkMRMLMarkupsClosedCurveNode") ||
       associatedNode->IsA("vtkMRMLMarkupsPlaneNode") ||
       associatedNode->IsA("vtkMRMLMarkupsROINode")))
    {
    // Item is a markup
    return 0.5;
    }

  return 0.0;
}

//---------------------------------------------------------------------------
const QString qSlicerSubjectHierarchyMarkupsPlugin::roleForPlugin()const
{
  return "Markup";
}

//---------------------------------------------------------------------------
QIcon qSlicerSubjectHierarchyMarkupsPlugin::icon(vtkIdType itemID)
{
  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return QIcon();
    }

  Q_D(qSlicerSubjectHierarchyMarkupsPlugin);

  if (!this->canOwnSubjectHierarchyItem(itemID))
    {
    // Item unknown by plugin
    return QIcon();
    }
  vtkMRMLSubjectHierarchyNode* shNode = qSlicerSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    return QIcon();
    }
  vtkMRMLNode* node = shNode->GetItemDataNode(itemID);
  if (!node)
    {
    return QIcon();
    }
  if (node->IsA("vtkMRMLMarkupsFiducialNode"))
    {
    return QIcon(":Icons/MarkupsFiducial.png");
    }
  else if (node->IsA("vtkMRMLMarkupsLineNode"))
    {
    return QIcon(":Icons/MarkupsLine.png");
    }
  else if (node->IsA("vtkMRMLMarkupsAngleNode"))
    {
    return QIcon(":Icons/MarkupsAngle.png");
    }
  else if (node->IsA("vtkMRMLMarkupsClosedCurveNode"))
    {
    // closed curve is a child class of curve node,
    return QIcon(":Icons/MarkupsClosedCurve.png");
    }
  else if (node->IsA("vtkMRMLMarkupsCurveNode"))
    {
    return QIcon(":Icons/MarkupsOpenCurve.png");
    }
  else if (node->IsA("vtkMRMLMarkupsPlaneNode"))
    {
    return QIcon(":Icons/MarkupsPlane.png");
    }
  else if (node->IsA("vtkMRMLMarkupsROINode"))
    {
    return QIcon(":Icons/MarkupsROI.png");
    }
  return QIcon();
}

//---------------------------------------------------------------------------
QIcon qSlicerSubjectHierarchyMarkupsPlugin::visibilityIcon(int visible)
{
  // Have the default plugin (which is not registered) take care of this
  return qSlicerSubjectHierarchyPluginHandler::instance()->defaultPlugin()->visibilityIcon(visible);
}

//-----------------------------------------------------------------------------
void qSlicerSubjectHierarchyMarkupsPlugin::setDisplayColor(vtkIdType itemID, QColor color, QMap<int, QVariant> terminologyMetaData)
{
  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return;
    }
  vtkMRMLSubjectHierarchyNode* shNode = qSlicerSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }

  // Get display node
  vtkMRMLDisplayableNode* displayableNode = vtkMRMLDisplayableNode::SafeDownCast(shNode->GetItemDataNode(itemID));
  if (!displayableNode)
    {
    qCritical() << Q_FUNC_INFO << ": Unable to find node for subject hierarchy item " << shNode->GetItemName(itemID).c_str();
    return;
    }
  vtkMRMLDisplayNode* displayNode = displayableNode->GetDisplayNode();
  if (!displayNode)
    {
    qCritical() << Q_FUNC_INFO << ": No display node";
    return;
    }

  // Set terminology metadata
  if (terminologyMetaData.contains(qSlicerTerminologyItemDelegate::TerminologyRole))
    {
    displayableNode->SetAttribute(vtkSegment::GetTerminologyEntryTagName(),
      terminologyMetaData[qSlicerTerminologyItemDelegate::TerminologyRole].toString().toUtf8().constData() );
    }
  if (terminologyMetaData.contains(qSlicerTerminologyItemDelegate::NameRole))
    {
    displayableNode->SetName(
      terminologyMetaData[qSlicerTerminologyItemDelegate::NameRole].toString().toUtf8().constData() );
    }
  if (terminologyMetaData.contains(qSlicerTerminologyItemDelegate::NameAutoGeneratedRole))
    {
    displayableNode->SetAttribute( vtkSlicerTerminologiesModuleLogic::GetNameAutoGeneratedAttributeName(),
      terminologyMetaData[qSlicerTerminologyItemDelegate::NameAutoGeneratedRole].toString().toUtf8().constData() );
    }
  if (terminologyMetaData.contains(qSlicerTerminologyItemDelegate::ColorAutoGeneratedRole))
    {
    displayableNode->SetAttribute( vtkSlicerTerminologiesModuleLogic::GetColorAutoGeneratedAttributeName(),
      terminologyMetaData[qSlicerTerminologyItemDelegate::ColorAutoGeneratedRole].toString().toUtf8().constData() );
    }

  // Set color
  double* oldColorArray = displayNode->GetColor();
  QColor oldColor = QColor::fromRgbF(oldColorArray[0], oldColorArray[1], oldColorArray[2]);
  if (oldColor != color)
    {
    displayNode->SetSelectedColor(color.redF(), color.greenF(), color.blueF());

    // Trigger update of color swatch
    shNode->ItemModified(itemID);
    }
}

//-----------------------------------------------------------------------------
QColor qSlicerSubjectHierarchyMarkupsPlugin::getDisplayColor(vtkIdType itemID, QMap<int, QVariant> &terminologyMetaData)const
{
  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return QColor(0,0,0,0);
    }
  vtkMRMLSubjectHierarchyNode* shNode = qSlicerSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return QColor(0,0,0,0);
    }
  vtkMRMLScene* scene = qSlicerSubjectHierarchyPluginHandler::instance()->mrmlScene();
  if (!scene)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid MRML scene";
    return QColor(0,0,0,0);
    }

  if (scene->IsImporting())
    {
    // During import SH node may be created before the segmentation is read into the scene,
    // so don't attempt to access the segment yet
    return QColor(0,0,0,0);
    }

  // Get display node
  vtkMRMLDisplayableNode* displayableNode = vtkMRMLDisplayableNode::SafeDownCast(shNode->GetItemDataNode(itemID));
  if (!displayableNode)
    {
    qCritical() << Q_FUNC_INFO << ": Unable to find node for subject hierarchy item " << shNode->GetItemName(itemID).c_str();
    return QColor(0,0,0,0);
    }
  vtkMRMLDisplayNode* displayNode = displayableNode->GetDisplayNode();
  if (!displayNode)
    {
    // this is normal when the markups node is being created
    return QColor(0,0,0,0);
    }

  // Get terminology metadata
  terminologyMetaData.clear();
  terminologyMetaData[qSlicerTerminologyItemDelegate::TerminologyRole] =
    displayableNode->GetAttribute(vtkSegment::GetTerminologyEntryTagName());
  terminologyMetaData[qSlicerTerminologyItemDelegate::NameRole] = displayableNode->GetName();
  // If auto generated flags are not initialized, then set them to the default
  // (color: on, name: off - this way color will be set from the selector but name will not)
  bool nameAutoGenerated = false;
  if (displayableNode->GetAttribute(vtkSlicerTerminologiesModuleLogic::GetNameAutoGeneratedAttributeName()))
    {
    nameAutoGenerated = QVariant(displayableNode->GetAttribute(vtkSlicerTerminologiesModuleLogic::GetNameAutoGeneratedAttributeName())).toBool();
    }
  terminologyMetaData[qSlicerTerminologyItemDelegate::NameAutoGeneratedRole] = nameAutoGenerated;
  bool colorAutoGenerated = true;
  if (displayableNode->GetAttribute(vtkSlicerTerminologiesModuleLogic::GetColorAutoGeneratedAttributeName()))
    {
    colorAutoGenerated = QVariant(displayableNode->GetAttribute(vtkSlicerTerminologiesModuleLogic::GetColorAutoGeneratedAttributeName())).toBool();
    }
  terminologyMetaData[qSlicerTerminologyItemDelegate::ColorAutoGeneratedRole] = colorAutoGenerated;

  // Get and return color
  double* colorArray = displayNode->GetSelectedColor();
  return QColor::fromRgbF(colorArray[0], colorArray[1], colorArray[2]);
}

//-----------------------------------------------------------------------------
QList<QAction*> qSlicerSubjectHierarchyMarkupsPlugin::viewContextMenuActions()const
{
  Q_D(const qSlicerSubjectHierarchyMarkupsPlugin);

  QList<QAction*> actions;
  //@by jzy
  //actions << d->RenamePointAction << d->DeletePointAction << d->ToggleSelectPointAction << d->ToggleHandleInteractive << d->HandleVisibilityAction;
  actions << d->DeletePointAction << d->ToggleHandleInteractive << d->HideLabelInteractive
      << d->ChangeColorInteractive << d->ChangeCameraDirectionInteractive
      << d->SliceProjectionInteractive << d->CommonSettingInteractive;
  //<< d->ResampleInteractive;
  return actions;
}

//-----------------------------------------------------------------------------
void qSlicerSubjectHierarchyMarkupsPlugin::showViewContextMenuActionsForItem(vtkIdType itemID, QVariantMap eventData)
{
  Q_D(qSlicerSubjectHierarchyMarkupsPlugin);

  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return;
    }
  vtkMRMLSubjectHierarchyNode* shNode = qSlicerSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }

  // Markup
  vtkMRMLMarkupsNode* associatedNode = vtkMRMLMarkupsNode::SafeDownCast(shNode->GetItemDataNode(itemID));
  if (associatedNode)
    {
    d->ViewMenuEventData = eventData;
    d->ViewMenuEventData["NodeID"] = QVariant(associatedNode->GetID());

    int componentType = d->ViewMenuEventData["ComponentType"].toInt();
    bool pointActionsDisabled =
      componentType == vtkMRMLMarkupsDisplayNode::ComponentTranslationHandle ||
      componentType == vtkMRMLMarkupsDisplayNode::ComponentRotationHandle ||
      componentType == vtkMRMLMarkupsDisplayNode::ComponentScaleHandle ||
      componentType == vtkMRMLMarkupsDisplayNode::ComponentPlane ||
      componentType == vtkMRMLMarkupsROIDisplayNode::ComponentROI;

    d->RenamePointAction->setVisible(!pointActionsDisabled);
    d->DeletePointAction->setVisible(!pointActionsDisabled);
    d->ToggleSelectPointAction->setVisible(!pointActionsDisabled);

    vtkMRMLMarkupsDisplayNode* displayNode = vtkMRMLMarkupsDisplayNode::SafeDownCast(associatedNode->GetDisplayNode());
    d->ToggleHandleInteractive->setVisible(displayNode != nullptr);
    d->HandleVisibilityAction->setVisible(displayNode != nullptr);
    d->HideLabelInteractive->setVisible(displayNode != nullptr);
    d->ChangeColorInteractive->setVisible(displayNode != nullptr);
    d->ChangeCameraDirectionInteractive->setVisible(displayNode != nullptr);
    d->SliceProjectionInteractive->setVisible(displayNode != nullptr);
    d->ResampleInteractive->setVisible(displayNode != nullptr);
    d->CommonSettingInteractive->setVisible(true);
    d->ToggleScaleHandleVisible->setVisible(vtkMRMLMarkupsROIDisplayNode::SafeDownCast(displayNode) != nullptr);
    if (displayNode)
      {
      d->ToggleHandleInteractive->setChecked(displayNode->GetHandlesInteractive());
      d->ToggleTranslateHandleVisible->setChecked(displayNode->GetTranslationHandleVisibility());
      d->ToggleRotateHandleVisible->setChecked(displayNode->GetRotationHandleVisibility());
      d->ToggleScaleHandleVisible->setChecked(displayNode->GetScaleHandleVisibility());
      }
    }
}

//-----------------------------------------------------------------------------
QList<QAction*> qSlicerSubjectHierarchyMarkupsPlugin::visibilityContextMenuActions() const
{
  Q_D(const qSlicerSubjectHierarchyMarkupsPlugin);

  QList<QAction*> actions;
  actions << d->ToggleCurrentItemHandleInteractive << d->CurrentItemHandleVisibilityAction;
  return actions;
}

//-----------------------------------------------------------------------------
void qSlicerSubjectHierarchyMarkupsPlugin::showVisibilityContextMenuActionsForItem(vtkIdType itemID)
{
  Q_D(qSlicerSubjectHierarchyMarkupsPlugin);

  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return;
    }
  vtkMRMLSubjectHierarchyNode* shNode = qSlicerSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }

  // Markup
  vtkMRMLMarkupsNode* associatedNode = vtkMRMLMarkupsNode::SafeDownCast(shNode->GetItemDataNode(itemID));
  if (associatedNode)
    {
    vtkMRMLMarkupsDisplayNode* displayNode = vtkMRMLMarkupsDisplayNode::SafeDownCast(associatedNode->GetDisplayNode());
    d->ToggleCurrentItemHandleInteractive->setVisible(displayNode != nullptr);
    d->CurrentItemHandleVisibilityAction->setVisible(displayNode != nullptr);
    d->ToggleCurrentItemScaleHandleVisible->setVisible(vtkMRMLMarkupsROIDisplayNode::SafeDownCast(displayNode) != nullptr);
    if (displayNode)
      {
      d->ToggleCurrentItemHandleInteractive->setChecked(displayNode->GetHandlesInteractive());
      d->ToggleCurrentItemTranslateHandleVisible->setChecked(displayNode->GetTranslationHandleVisibility());
      d->ToggleCurrentItemRotateHandleVisible->setChecked(displayNode->GetRotationHandleVisibility());
      d->ToggleCurrentItemScaleHandleVisible->setChecked(displayNode->GetScaleHandleVisibility());
      }
    }
}

//-----------------------------------------------------------------------------
void qSlicerSubjectHierarchyMarkupsPlugin::renamePoint()
{
  Q_D(qSlicerSubjectHierarchyMarkupsPlugin);

  if (d->ViewMenuEventData.find("NodeID") == d->ViewMenuEventData.end())
    {
    qCritical() << Q_FUNC_INFO << ": No node ID found in the view menu event data";
    return;
    }
  vtkMRMLScene* scene = qSlicerSubjectHierarchyPluginHandler::instance()->mrmlScene();
  if (!scene)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access MRML scene";
    return;
    }

  // Get markups node
  QString nodeID = d->ViewMenuEventData["NodeID"].toString();
  vtkMRMLNode* node = scene->GetNodeByID(nodeID.toUtf8().constData());
  vtkMRMLMarkupsNode* markupsNode = vtkMRMLMarkupsNode::SafeDownCast(node);
  if (!markupsNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to get markups node by ID " << nodeID;
    return;
    }

  // Get point index
  int componentIndex = d->ViewMenuEventData["ComponentIndex"].toInt();

  // Pop up an entry box for the new name, with the old name as default
  QString oldName(markupsNode->GetNthControlPointLabel(componentIndex).c_str());

  bool ok = false;
  QString newName = QInputDialog::getText(nullptr, QString("Rename ") + oldName, "New name:", QLineEdit::Normal, oldName, &ok);
  if (!ok)
    {
    return;
    }

  markupsNode->SetNthControlPointLabel(componentIndex, newName.toUtf8().constData());
}

//-----------------------------------------------------------------------------
void qSlicerSubjectHierarchyMarkupsPlugin::deletePoint()
{
  Q_D(qSlicerSubjectHierarchyMarkupsPlugin);

  if (d->ViewMenuEventData.find("NodeID") == d->ViewMenuEventData.end())
    {
    qCritical() << Q_FUNC_INFO << ": No node ID found in the view menu event data";
    return;
    }
  vtkMRMLScene* scene = qSlicerSubjectHierarchyPluginHandler::instance()->mrmlScene();
  if (!scene)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access MRML scene";
    return;
    }

  // Get markups node
  QString nodeID = d->ViewMenuEventData["NodeID"].toString();
  vtkMRMLNode* node = scene->GetNodeByID(nodeID.toUtf8().constData());
  vtkMRMLMarkupsNode* markupsNode = vtkMRMLMarkupsNode::SafeDownCast(node);
  if (!markupsNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to get markups node by ID " << nodeID;
    return;
    }

  // Get point index
  int componentIndex = d->ViewMenuEventData["ComponentIndex"].toInt();

  //markupsNode->RemoveNthControlPoint(componentIndex);
  markupsNode->RemoveAllControlPoints();
  QTimer::singleShot(10, [=]() {
      scene->RemoveNode(markupsNode);
      });
}

//-----------------------------------------------------------------------------
void qSlicerSubjectHierarchyMarkupsPlugin::toggleSelectPoint()
{
  Q_D(qSlicerSubjectHierarchyMarkupsPlugin);

  if (d->ViewMenuEventData.find("NodeID") == d->ViewMenuEventData.end())
    {
    qCritical() << Q_FUNC_INFO << ": No node ID found in the view menu event data";
    return;
    }
  vtkMRMLScene* scene = qSlicerSubjectHierarchyPluginHandler::instance()->mrmlScene();
  if (!scene)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access MRML scene";
    return;
    }

  // Get markups node
  QString nodeID = d->ViewMenuEventData["NodeID"].toString();
  vtkMRMLNode* node = scene->GetNodeByID(nodeID.toUtf8().constData());
  vtkMRMLMarkupsNode* markupsNode = vtkMRMLMarkupsNode::SafeDownCast(node);
  if (!markupsNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to get markups node by ID " << nodeID;
    return;
    }

  // Get point index
  int componentIndex = d->ViewMenuEventData["ComponentIndex"].toInt();

  markupsNode->SetNthControlPointSelected(componentIndex, !markupsNode->GetNthControlPointSelected(componentIndex));
}

void qSlicerSubjectHierarchyMarkupsPlugin::on_rotate_to_vector(double* m_PointInput, double* m_PointTarget) {
    double dx = m_PointTarget[0];
    double dy = m_PointTarget[1];
    double dz = m_PointTarget[2];



    double vector1[3];
    vector1[0] = m_PointInput[0] - m_PointTarget[0];
    vector1[1] = m_PointInput[1] - m_PointTarget[1];
    vector1[2] = m_PointInput[2] - m_PointTarget[2];
    double vector0[3];
    vector0[0] = 0;
    vector0[1] = 0;
    vector0[2] = 1;
    double rotationVector_Local[3] = { 0.0 };
    double angle = vtkMath::DegreesFromRadians(vtkMath::AngleBetweenVectors(vector0, vector1));
    vtkMath::Cross(vector0, vector1, rotationVector_Local);
    vtkNew<vtkMatrix4x4> modelToParentTransform;
    modelToParentTransform->Identity();
    modelToParentTransform->SetElement(0, 3, m_PointTarget[0]);
    modelToParentTransform->SetElement(1, 3, m_PointTarget[1]);
    modelToParentTransform->SetElement(2, 3, m_PointTarget[2]);
    vtkNew<vtkTransform> handleToWorldMatrix;
    handleToWorldMatrix->PostMultiply();
    handleToWorldMatrix->Concatenate(modelToParentTransform);
    handleToWorldMatrix->Translate(-m_PointTarget[0], -m_PointTarget[1], -m_PointTarget[2]);
    handleToWorldMatrix->RotateWXYZ(angle, rotationVector_Local);
    handleToWorldMatrix->Translate(m_PointTarget);
    modelToParentTransform->DeepCopy(handleToWorldMatrix->GetMatrix());


    typedef std::map<vtkSmartPointer<vtkMRMLNode>, qMRMLViewControllerBar* > ControllerMapType;
    ControllerMapType::iterator cit;
    auto lm = qSlicerApplication::application()->layoutManager();
    auto sliceNames = lm->sliceViewNames();
    for (int i = 0; i < sliceNames.size(); i++) {
        vtkMRMLSliceNode* sliceNode = lm->sliceWidget(sliceNames.at(i))->mrmlSliceNode();
        vtkNew<vtkMatrix4x4> ijkToRAS;
        sliceNode->SetOrientationToDefault();
        vtkMatrix4x4* sliceToRAS = sliceNode->GetSliceToRAS();
        sliceToRAS->Multiply4x4(modelToParentTransform, sliceToRAS, sliceToRAS);
        sliceNode->GetSliceToRAS()->DeepCopy(sliceToRAS);
        sliceNode->UpdateMatrices();
    }

    for (int i = 0; i < sliceNames.size(); i++) {
        auto controller = lm->sliceWidget(sliceNames.at(i))->sliceController();
        auto node = lm->sliceWidget(sliceNames.at(i))->mrmlSliceNode();
        controller->fitSliceToBackground();
    }
    lm->resetThreeDViews();
    vtkMRMLSliceNode::JumpAllSlices(qSlicerSubjectHierarchyPluginHandler::instance()->mrmlScene(), dx, dy, dz, 1);
}

void qSlicerSubjectHierarchyMarkupsPlugin::toggleCommonSettingInteractive() {
    auto settingPanel = qSlicerApplication::application()->settingsDialog();
    settingPanel->showNormal();
    settingPanel->setCurrentPanel(QStringLiteral("测量"));
    return;
}

void qSlicerSubjectHierarchyMarkupsPlugin::toggleResampleInteractive() {
    Q_D(qSlicerSubjectHierarchyMarkupsPlugin);
    if (d->ViewMenuEventData.find("NodeID") == d->ViewMenuEventData.end())
    {
        qCritical() << Q_FUNC_INFO << ": No node ID found in the view menu event data";
        return;
    }
    vtkMRMLScene* scene = qSlicerSubjectHierarchyPluginHandler::instance()->mrmlScene();
    if (!scene)
    {
        qCritical() << Q_FUNC_INFO << ": Failed to access MRML scene";
        return;
    }

    // Get markups node
    QString nodeID = d->ViewMenuEventData["NodeID"].toString();
    vtkMRMLNode* node = scene->GetNodeByID(nodeID.toUtf8().constData());
    vtkMRMLMarkupsNode* markupsNode = vtkMRMLMarkupsNode::SafeDownCast(node);
    if (!markupsNode)
    {
        qCritical() << Q_FUNC_INFO << ": Failed to get markups node by ID " << nodeID;
        return;
    }

    vtkMRMLMarkupsDisplayNode* displayNode = vtkMRMLMarkupsDisplayNode::SafeDownCast(markupsNode->GetDisplayNode());
    if (!displayNode)
    {
        qCritical() << Q_FUNC_INFO << ": Failed to get display node for " << nodeID;
        return;
    }

    int resampleNumberOfPoints = 21;
    auto outputNode = vtkMRMLMarkupsCurveNode::SafeDownCast(markupsNode);
    double sampleDist = outputNode->GetCurveLengthWorld() / (resampleNumberOfPoints - 1.0);
    outputNode->ResampleCurveWorld(sampleDist);
}

void qSlicerSubjectHierarchyMarkupsPlugin::toggleSliceProjectionInteractive() {
    Q_D(qSlicerSubjectHierarchyMarkupsPlugin);
    if (d->ViewMenuEventData.find("NodeID") == d->ViewMenuEventData.end())
    {
        qCritical() << Q_FUNC_INFO << ": No node ID found in the view menu event data";
        return;
    }
    vtkMRMLScene* scene = qSlicerSubjectHierarchyPluginHandler::instance()->mrmlScene();
    if (!scene)
    {
        qCritical() << Q_FUNC_INFO << ": Failed to access MRML scene";
        return;
    }

    // Get markups node
    QString nodeID = d->ViewMenuEventData["NodeID"].toString();
    vtkMRMLNode* node = scene->GetNodeByID(nodeID.toUtf8().constData());
    vtkMRMLMarkupsNode* markupsNode = vtkMRMLMarkupsNode::SafeDownCast(node);
    if (!markupsNode)
    {
        qCritical() << Q_FUNC_INFO << ": Failed to get markups node by ID " << nodeID;
        return;
    }

    vtkMRMLMarkupsDisplayNode* displayNode = vtkMRMLMarkupsDisplayNode::SafeDownCast(markupsNode->GetDisplayNode());
    if (!displayNode)
    {
        qCritical() << Q_FUNC_INFO << ": Failed to get display node for " << nodeID;
        return;
    }
    displayNode->SetSliceProjection(!displayNode->GetSliceProjection());
    double color[3];
    color[0] = 1;
    color[1] = 1;
    color[2] = 1;
    displayNode->SetSliceProjectionColor(color);
    displayNode->SetSliceProjectionOpacity(0.3);
}


void qSlicerSubjectHierarchyMarkupsPlugin::toggleChangeCameraDirectionInteractive() {
    Q_D(qSlicerSubjectHierarchyMarkupsPlugin);
    
    if (d->ViewMenuEventData.find("NodeID") == d->ViewMenuEventData.end())
    {
        qCritical() << Q_FUNC_INFO << ": No node ID found in the view menu event data";
        return;
    }
    vtkMRMLScene* scene = qSlicerSubjectHierarchyPluginHandler::instance()->mrmlScene();
    if (!scene)
    {
        qCritical() << Q_FUNC_INFO << ": Failed to access MRML scene";
        return;
    }

    // Get markups node
    QString nodeID = d->ViewMenuEventData["NodeID"].toString();
    vtkMRMLNode* node = scene->GetNodeByID(nodeID.toUtf8().constData());
    vtkMRMLMarkupsNode* markupsNode = vtkMRMLMarkupsNode::SafeDownCast(node);
    if (!markupsNode)
    {
        qCritical() << Q_FUNC_INFO << ": Failed to get markups node by ID " << nodeID;
        return;
    }

    vtkMRMLMarkupsDisplayNode* displayNode = vtkMRMLMarkupsDisplayNode::SafeDownCast(markupsNode->GetDisplayNode());
    if (!displayNode)
    {
        qCritical() << Q_FUNC_INFO << ": Failed to get display node for " << nodeID;
        return;
    }

    vtkMRMLMarkupsLineNode* lineNode = vtkMRMLMarkupsLineNode::SafeDownCast(markupsNode);
    //if (lineNode) 
    {
        std::vector< vtkMRMLMarkupsNode::ControlPoint* >* nodes = markupsNode->GetControlPoints();
        if (nodes->size() < 2) {
            return;
        }
        vtkMRMLMarkupsNode::ControlPoint* node0 = nodes->at(0);
        vtkMRMLMarkupsNode::ControlPoint* node1 = nodes->at(1);
        on_rotate_to_vector(node0->Position, node1->Position);
    }
}

void qSlicerSubjectHierarchyMarkupsPlugin::toggleChangeColorInteractive() {
    Q_D(qSlicerSubjectHierarchyMarkupsPlugin);

    if (d->ViewMenuEventData.find("NodeID") == d->ViewMenuEventData.end())
    {
        qCritical() << Q_FUNC_INFO << ": No node ID found in the view menu event data";
        return;
    }
    vtkMRMLScene* scene = qSlicerSubjectHierarchyPluginHandler::instance()->mrmlScene();
    if (!scene)
    {
        qCritical() << Q_FUNC_INFO << ": Failed to access MRML scene";
        return;
    }

    // Get markups node
    QString nodeID = d->ViewMenuEventData["NodeID"].toString();
    vtkMRMLNode* node = scene->GetNodeByID(nodeID.toUtf8().constData());
    vtkMRMLMarkupsNode* markupsNode = vtkMRMLMarkupsNode::SafeDownCast(node);
    if (!markupsNode)
    {
        qCritical() << Q_FUNC_INFO << ": Failed to get markups node by ID " << nodeID;
        return;
    }

    vtkMRMLMarkupsDisplayNode* displayNode = vtkMRMLMarkupsDisplayNode::SafeDownCast(markupsNode->GetDisplayNode());
    if (!displayNode)
    {
        qCritical() << Q_FUNC_INFO << ": Failed to get display node for " << nodeID;
        return;
    }

    if (!d->m_pColorDialog) {
        d->m_pColorDialog = new QColorDialog();
        QObject::connect(d->m_pColorDialog, &QColorDialog::colorSelected, this, [&](QColor color) {
            auto m_red = color.red();
            auto m_green = color.green();
            auto m_blue = color.blue();
            double Color[4];
            Color[0] = m_red / 255.0;
            Color[1] = m_green / 255.0;
            Color[2] = m_blue / 255.0;

            displayNode->SetSelectedColor(Color);
            });
    }
    d->m_pColorDialog->exec();
}

void qSlicerSubjectHierarchyMarkupsPlugin::toggleHideLabelInteractive() {
    Q_D(qSlicerSubjectHierarchyMarkupsPlugin);

    if (d->ViewMenuEventData.find("NodeID") == d->ViewMenuEventData.end())
    {
        qCritical() << Q_FUNC_INFO << ": No node ID found in the view menu event data";
        return;
    }
    vtkMRMLScene* scene = qSlicerSubjectHierarchyPluginHandler::instance()->mrmlScene();
    if (!scene)
    {
        qCritical() << Q_FUNC_INFO << ": Failed to access MRML scene";
        return;
    }

    // Get markups node
    QString nodeID = d->ViewMenuEventData["NodeID"].toString();
    vtkMRMLNode* node = scene->GetNodeByID(nodeID.toUtf8().constData());
    vtkMRMLMarkupsNode* markupsNode = vtkMRMLMarkupsNode::SafeDownCast(node);
    if (!markupsNode)
    {
        qCritical() << Q_FUNC_INFO << ": Failed to get markups node by ID " << nodeID;
        return;
    }

    vtkMRMLMarkupsDisplayNode* displayNode = vtkMRMLMarkupsDisplayNode::SafeDownCast(markupsNode->GetDisplayNode());
    if (!displayNode)
    {
        qCritical() << Q_FUNC_INFO << ": Failed to get display node for " << nodeID;
        return;
    }
    displayNode->SetPropertiesLabelVisibility(!displayNode->GetPropertiesLabelVisibility());
}

//-----------------------------------------------------------------------------
void qSlicerSubjectHierarchyMarkupsPlugin::toggleHandleInteractive()
{
  Q_D(qSlicerSubjectHierarchyMarkupsPlugin);

  if (d->ViewMenuEventData.find("NodeID") == d->ViewMenuEventData.end())
    {
    qCritical() << Q_FUNC_INFO << ": No node ID found in the view menu event data";
    return;
    }
  vtkMRMLScene* scene = qSlicerSubjectHierarchyPluginHandler::instance()->mrmlScene();
  if (!scene)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access MRML scene";
    return;
    }

  // Get markups node
  QString nodeID = d->ViewMenuEventData["NodeID"].toString();
  vtkMRMLNode* node = scene->GetNodeByID(nodeID.toUtf8().constData());
  vtkMRMLMarkupsNode* markupsNode = vtkMRMLMarkupsNode::SafeDownCast(node);
  if (!markupsNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to get markups node by ID " << nodeID;
    return;
    }

  vtkMRMLMarkupsDisplayNode* displayNode = vtkMRMLMarkupsDisplayNode::SafeDownCast(markupsNode->GetDisplayNode());
  if (!displayNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to get display node for " << nodeID;
    return;
    }
  displayNode->SetHandlesInteractive(!displayNode->GetHandlesInteractive());
}

//-----------------------------------------------------------------------------
void qSlicerSubjectHierarchyMarkupsPlugin::toggleCurrentItemHandleInteractive()
{
  Q_D(qSlicerSubjectHierarchyMarkupsPlugin);

  // Get currently selected node and scene
  vtkMRMLSubjectHierarchyNode* shNode = qSlicerSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }
  vtkIdType currentItemID = qSlicerSubjectHierarchyPluginHandler::instance()->currentItem();
  if (!currentItemID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid current subject hierarchy item";
    return;
    }

  vtkMRMLMarkupsNode* markupNode = vtkMRMLMarkupsNode::SafeDownCast(shNode->GetItemDataNode(currentItemID));
  if (!markupNode)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid markup node";
    return;
    }

  markupNode->CreateDefaultDisplayNodes();
  vtkMRMLMarkupsDisplayNode* displayNode = vtkMRMLMarkupsDisplayNode::SafeDownCast(markupNode->GetDisplayNode());
  if (!displayNode)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid display node";
    return;
    }

  displayNode->SetHandlesInteractive(!displayNode->GetHandlesInteractive());
}

//-----------------------------------------------------------------------------
void qSlicerSubjectHierarchyMarkupsPlugin::toggleCurrentItemHandleTypeVisibility()
{
  Q_D(qSlicerSubjectHierarchyMarkupsPlugin);

  if (d->ViewMenuEventData.find("NodeID") == d->ViewMenuEventData.end())
    {
    qCritical() << Q_FUNC_INFO << ": No node ID found in the view menu event data";
    return;
    }
  vtkMRMLScene* scene = qSlicerSubjectHierarchyPluginHandler::instance()->mrmlScene();
  if (!scene)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access MRML scene";
    return;
    }

  // Get markups node
  QString nodeID = d->ViewMenuEventData["NodeID"].toString();
  vtkMRMLNode* node = scene->GetNodeByID(nodeID.toUtf8().constData());
  vtkMRMLMarkupsNode* markupsNode = vtkMRMLMarkupsNode::SafeDownCast(node);
  if (!markupsNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to get markups node by ID " << nodeID;
    return;
    }

  vtkMRMLMarkupsDisplayNode* displayNode = vtkMRMLMarkupsDisplayNode::SafeDownCast(markupsNode->GetDisplayNode());
  if (!displayNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to get display node for " << nodeID;
    return;
    }

  QObject* sender = QObject::sender();
  if (!sender)
    {
    return;
    }

  int componentType = sender->property(INTERACTION_HANDLE_TYPE_PROPERTY).toInt();
  displayNode->SetHandleVisibility(componentType, !displayNode->GetHandleVisibility(componentType));
}

//-----------------------------------------------------------------------------
void qSlicerSubjectHierarchyMarkupsPlugin::toggleHandleTypeVisibility()
{
  Q_D(qSlicerSubjectHierarchyMarkupsPlugin);

  if (d->ViewMenuEventData.find("NodeID") == d->ViewMenuEventData.end())
    {
    qCritical() << Q_FUNC_INFO << ": No node ID found in the view menu event data";
    return;
    }
  vtkMRMLScene* scene = qSlicerSubjectHierarchyPluginHandler::instance()->mrmlScene();
  if (!scene)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access MRML scene";
    return;
    }

  // Get markups node
  QString nodeID = d->ViewMenuEventData["NodeID"].toString();
  vtkMRMLNode* node = scene->GetNodeByID(nodeID.toUtf8().constData());
  vtkMRMLMarkupsNode* markupsNode = vtkMRMLMarkupsNode::SafeDownCast(node);
  if (!markupsNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to get markups node by ID " << nodeID;
    return;
    }

  vtkMRMLMarkupsDisplayNode* displayNode = vtkMRMLMarkupsDisplayNode::SafeDownCast(markupsNode->GetDisplayNode());
  if (!displayNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to get display node for " << nodeID;
    return;
    }

  QObject* sender = QObject::sender();
  if (!sender)
    {
    return;
    }

  int componentType = sender->property(INTERACTION_HANDLE_TYPE_PROPERTY).toInt();
  displayNode->SetHandleVisibility(componentType, !displayNode->GetHandleVisibility(componentType));
}
