/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qSlicerGLModuleWidget_h
#define __qSlicerGLModuleWidget_h


// Slicer includes
#include "qSlicerApplication.h"
#include "vtkMRMLScene.h"
#include "qMRMLViewControllerBar.h"
#include  "qSlicerAbstractModuleWidget.h"
#include "vtkSmartPointer.h"
#include "qSlicerLayoutManager.h"
#include "vtkMRMLSliceNode.h"
#include "qMRMLSliceControllerWidget.h"
#include "vtkMRMLViewNode.h"
#include "qMRMLThreeDWidget.h"
#include "qMRMLThreeDViewControllerWidget.h"
#include "qMRMLSliceWidget.h"
#include "vtkMRMLScalarVolumeNode.h"
#include "vtkMRMLColorNode.h"
#include "qevent.h"
#include "qmainwindow.h"
#include "vtkCollection.h"
#include "qSlicerAppExport.h"
//@by jzy
class qSlicerGLModuleWidgetPrivate;
class vtkMRMLNode;
class qSlicerMainWindow;
#define  ATB(i) if(AutoTestBreak(i)){return;}

///
/// Base class of all the Slicer module widgets. The widget is added in the module panels.
/// Deriving from qSlicerWidget, it inherits the mrmlScene()/setMRMLScene() methods.
class Q_SLICER_APP_EXPORT qSlicerGLModuleWidget
  :public qSlicerAbstractModuleWidget
{
	Q_OBJECT
public:

public:
	
	//QMap<const char*, QString> m_GlobalMap;
	void DoResetGlobalParas();
	qSlicerGLModuleWidget(QWidget* parent = 0);
	virtual ~qSlicerGLModuleWidget();
	void DoSelectVolumeNode(vtkMRMLNode* node);
	void DoSelectVolumeNode(vtkMRMLNode* node, vtkMRMLNode* back_node, double opacity);
	void DoSetOpacityRatio(int value);
	vtkMRMLScalarVolumeNode* GetVolume2D(const char* alias_name, bool is_registed = false) {
		vtkCollection* nodes  = mrmlScene()->GetNodes();
		for (int n = 0; n < nodes->GetNumberOfItems(); n++)
		{
			vtkMRMLScalarVolumeNode* node = vtkMRMLScalarVolumeNode::SafeDownCast((vtkMRMLNode*)nodes->GetItemAsObject(n));
			if (!node) {
				continue;
			}
			if (node->GetAttribute("alias_name")&&strcmp(node->GetAttribute("alias_name"), alias_name) == 0) {
				if (is_registed) {
					if (node->GetAttribute("is_registed") != nullptr) {
						return node;
					}
				}
				else {
					if (node->GetAttribute("is_registed") == nullptr) {
						return node;
					}
				}
			}
		}
		return nullptr;
	}
	void SafeRemove(vtkMRMLNode* node);
	bool AutoTestBreak(int i) { return false; }
	
public slots:
	
protected:
	QScopedPointer<qSlicerGLModuleWidgetPrivate> d_ptr;
	typedef std::map<vtkSmartPointer<vtkMRMLNode>, qMRMLViewControllerBar* > ControllerMapType;
	//ControllerMapType ControllerMap;
	void AddSignalNode(const char* name, const char* data);
	//void  qSlicerGLModuleWidget::createController(vtkMRMLNode* n, qSlicerLayoutManager* layoutManager);
	virtual void setup();
	//add a signal node to MRMLScene with a single data 
	//used to send data to other project
	
	
	//add a signal node to MRMLScene with a data list
	void AddSignalNode(const char* name, QStringList list);
	const char* GetGlobalSaveValue(const char* key);
	std::vector< std::string > GetGlobalKeys();
	void SetGlobalSaveValue(const char* key,const char* val);
	std::vector<vtkMRMLNode*>* GetNodesByAttribute(const char* key, const char* val);
	void Reinit();
	
	
private:
	Q_DECLARE_PRIVATE(qSlicerGLModuleWidget);
	Q_DISABLE_COPY(qSlicerGLModuleWidget);
};

#endif
