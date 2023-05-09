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

// Slicer includes
#include "qSlicerGLModuleWidget.h"
#include "vtkMRMLSliceCompositeNode.h"
#include "vtkMRMLColorNode.h"
#include "itksys/SystemTools.hxx"
#include "vtkCollection.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerGLModuleWidgetPrivate 
{
public:
	qSlicerGLModuleWidgetPrivate();
};

//-----------------------------------------------------------------------------
// qSlicerGLModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerGLModuleWidgetPrivate::qSlicerGLModuleWidgetPrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerGLModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerGLModuleWidget::qSlicerGLModuleWidget(QWidget* _parent)
	: qSlicerAbstractModuleWidget(_parent)
	, d_ptr(new qSlicerGLModuleWidgetPrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerGLModuleWidget::~qSlicerGLModuleWidget()
{ 
}


//void
//qSlicerGLModuleWidget::createController(vtkMRMLNode* n, qSlicerLayoutManager* layoutManager)
//{
//	if (this->ControllerMap.find(n) != this->ControllerMap.end())
//	{
//		qDebug() << "qSlicerViewControllersModuleWidgetPrivate::createController - Node already added to module";
//		return;
//	}
//	// create the ControllerWidget and wire it to the appropriate node
//	qMRMLViewControllerBar* barWidget = nullptr;
//	vtkMRMLSliceNode* sn = vtkMRMLSliceNode::SafeDownCast(n);
//	if (sn)
//	{
//		qMRMLSliceWidget* sliceWidget = layoutManager->sliceWidget(sn->GetLayoutName());
//		qMRMLSliceControllerWidget* widget =
//			new qMRMLSliceControllerWidget(nullptr);
//		widget->setSliceViewName(sn->GetName()); // call before setting slice node
//		widget->setSliceViewLabel(sn->GetLayoutLabel());
//		QColor layoutColor = QColor::fromRgbF(sn->GetLayoutColor()[0],
//			sn->GetLayoutColor()[1],
//			sn->GetLayoutColor()[2]);
//		widget->setSliceViewColor(layoutColor);
//		widget->setMRMLSliceNode(sn);
//		widget->setLayoutBehavior(qMRMLViewControllerBar::Panel);
//
//		// SliceControllerWidget needs to know the SliceLogic(s)
//		
//		widget->setSliceLogics(layoutManager->mrmlSliceLogics());
//		widget->setSliceLogic(sliceWidget->sliceController()->sliceLogic());
//		widget->setSliceLink(true);
//		barWidget = widget;
//	}
//
//	vtkMRMLViewNode* vn = vtkMRMLViewNode::SafeDownCast(n);
//	if (vn)
//	{
//		// ThreeDViewController needs to now the ThreeDView
//		qMRMLThreeDWidget* viewWidget = dynamic_cast<qMRMLThreeDWidget*>(layoutManager->viewWidget(vn));
//		if (viewWidget)
//		{
//			qMRMLThreeDViewControllerWidget* widget =
//				new qMRMLThreeDViewControllerWidget(nullptr);
//			widget->setLayoutBehavior(qMRMLViewControllerBar::Panel);
//			widget->setMRMLScene(mrmlScene());
//			widget->setThreeDView(viewWidget->threeDView());
//			widget->setViewLink(true);
//			widget->set3DAxisVisible(false);
//			widget->set3DAxisLabelVisible(false);
//			// qMRMLThreeDViewControllerWidget needs to know the ViewLogic(s)
//			widget->setViewLogic(viewWidget->threeDController()->viewLogic());
//			barWidget = widget;
//		}
//	}
//	this->ControllerMap[n] = barWidget;
//	
//}

std::vector<vtkMRMLNode*>* qSlicerGLModuleWidget::GetNodesByAttribute(const char* key, const char* val) {
	std::vector<vtkMRMLNode*>* vector1 = new std::vector<vtkMRMLNode*>();
	vtkMRMLNode* node = nullptr;
	vtkCollectionSimpleIterator it;
	vtkCollection* sceneNodes = mrmlScene()->GetNodes();
	for (sceneNodes->InitTraversal(it);
		(node = vtkMRMLNode::SafeDownCast(sceneNodes->GetNextItemAsObject(it)));) {
		const char* m_tmp = node->GetAttribute(key);
		if (m_tmp && strcmp(val, m_tmp) == 0) {
			vector1->push_back(node);
		}

	}
	return vector1;
}

void qSlicerGLModuleWidget::DoResetGlobalParas() {
	//m_GlobalMap[G::BasicNodeChangeFlag] = "false";
}

void qSlicerGLModuleWidget::SafeRemove(vtkMRMLNode* node) {

	if (node) {
		if (mrmlScene()->IsNodePresent(node) == 0) {
			return;
		}
		else {
			mrmlScene()->RemoveNode(node);
		}
	}
}

const char* qSlicerGLModuleWidget::GetGlobalSaveValue(const char* key)
{
	vtkMRMLNode* node = mrmlScene()->GetFirstNodeByName("saveconfig");
	
	if (!node) {
		return nullptr;
	}
	auto id = node->GetID();
	const char* val = node->GetAttribute(key);
	return val;
}

void qSlicerGLModuleWidget::SetGlobalSaveValue(const char* key, const char* val) {
	vtkMRMLNode* node = mrmlScene()->GetFirstNodeByName("saveconfig");
	
	if (!node) {
		node = mrmlScene()->CreateNodeByClass("vtkMRMLTransformNode");
		node->SetName("saveconfig");
		mrmlScene()->AddNode(node);
	}
	auto id = node->GetID();
	node->SetAttribute(key, val);
}



void qSlicerGLModuleWidget::AddSignalNode(const char* name, const char* data)
{
	vtkMRMLColorNode* node = vtkMRMLColorNode::New();
	node->SetAttribute("Help_Node", "true");
	node->SetAttribute("Help_Name", name);
	node->SetAttribute("Help_Info_0", data);
	mrmlScene()->AddNode(node);
}

std::vector< std::string > qSlicerGLModuleWidget::GetGlobalKeys() {
	vtkMRMLNode* node = mrmlScene()->GetFirstNodeByName("saveconfig");
	if (!node) {
		std::vector< std::string > a;
		return a;
	}
	return node->GetAttributeNames();
}

void qSlicerGLModuleWidget::AddSignalNode(const char* name, QStringList list)
{
	vtkMRMLColorNode* node = vtkMRMLColorNode::New();
	node->SetAttribute("Help_Node", "true");
	node->SetAttribute("Help_Name", name);
	for (int i = 0; i < list.length(); i++) {
		QString info = QString("Help_Info_%1").arg(i);
		node->SetAttribute(info.toStdString().c_str(), list[i].toStdString().c_str());
	}
	mrmlScene()->AddNode(node);
}


void qSlicerGLModuleWidget::Reinit() {
	ControllerMapType::iterator cit;
	auto lm = qSlicerApplication::application()->layoutManager();
	auto sliceNames = lm->sliceViewNames();
	for (int i = 0; i < sliceNames.size(); i++) {
		auto controller = lm->sliceWidget(sliceNames.at(i))->sliceController();
		auto node = lm->sliceWidget(sliceNames.at(i))->mrmlSliceNode();
		node->SetOrientationToDefault();
		controller->fitSliceToBackground();
	}
	lm->resetThreeDViews();
}

void qSlicerGLModuleWidget::DoSelectVolumeNode(vtkMRMLNode* node) {
	if (!node) {
		return;
	}
	auto nodesNum = mrmlScene()->GetNumberOfNodesByClass("vtkMRMLSliceCompositeNode");
	for (int i = 0; i < nodesNum;i++) {
		auto sliceViewer = vtkMRMLSliceCompositeNode::SafeDownCast(mrmlScene()->GetNthNodeByClass(i, "vtkMRMLSliceCompositeNode"));
		sliceViewer->SetBackgroundVolumeID(node->GetID());
		sliceViewer->SetForegroundOpacity(1.0);
		sliceViewer->SetForegroundVolumeID(nullptr);
		sliceViewer->SetLabelVolumeID(nullptr);
	}
	//Reinit();
}

void qSlicerGLModuleWidget::DoSelectVolumeNode(vtkMRMLNode* node, vtkMRMLNode* back_node, double opacity) {
	auto nodesNum = mrmlScene()->GetNumberOfNodesByClass("vtkMRMLSliceCompositeNode");
	for (int i = 0; i < nodesNum; i++) {
		auto sliceViewer = vtkMRMLSliceCompositeNode::SafeDownCast(mrmlScene()->GetNthNodeByClass(i, "vtkMRMLSliceCompositeNode"));
		sliceViewer->SetBackgroundVolumeID(back_node->GetID());
		sliceViewer->SetForegroundOpacity(opacity);
		sliceViewer->SetForegroundVolumeID(node->GetID());
		sliceViewer->SetLabelVolumeID(nullptr);
	}
}

void qSlicerGLModuleWidget::DoSetOpacityRatio(int value) {
	auto nodesNum = mrmlScene()->GetNumberOfNodesByClass("vtkMRMLSliceCompositeNode");
	for (int i = 0; i < nodesNum; i++) {
		auto sliceViewer = vtkMRMLSliceCompositeNode::SafeDownCast(mrmlScene()->GetNthNodeByClass(i, "vtkMRMLSliceCompositeNode"));
		sliceViewer->SetForegroundOpacity(value/100.0);
		sliceViewer->SetLabelVolumeID(nullptr);
	}
}

//-----------------------------------------------------------------------------
void qSlicerGLModuleWidget::setup()
{
	Q_D(qSlicerGLModuleWidget);
}

