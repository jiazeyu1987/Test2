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

// .NAME vtkSlicerMeasureLogicAbstract - slicer logic class for volumes manipulation
// .SECTION Description
// This class manages the logic associated with reading, saving,
// and changing propertied of the volumes


#ifndef __vtkSlicerMeasureLogic_h
#define __vtkSlicerMeasureLogic_h

// Slicer includes
#include "vtkSlicerModuleLogic.h"

// MRML includes

// STD includes
#include <cstdlib>

#include "vtkMRMLSelectionNode.h"
#include "vtkMRMLInteractionNode.h"
#include "vtkMRMLDisplayNode.h"
#include "vtkMRMLSliceCompositeNode.h"
#include "vtkMRMLSliceNode.h"
// SlicerLogic includes
#include "vtkSlicerApplicationLogic.h"
#include "qSlicerAppExport.h"
// MRMLLogic includes
#include <vtkMRMLAbstractLogic.h>
/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_APP_EXPORT vtkSlicerMeasureLogicAbstract :
  public vtkSlicerModuleLogic
{
public: 

  static vtkSlicerMeasureLogicAbstract *New();
  vtkTypeMacro(vtkSlicerMeasureLogicAbstract, vtkSlicerModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent);
  void SetActiveListID(vtkMRMLNode* markupsNode);
  std::string AddNewDisplayNodeForMarkupsNode(vtkMRMLNode* mrmlNode);
  void SetDisplayNodeToDefaults(vtkMRMLDisplayNode* displayNode);
  std::string GetSelectionNodeID();
  void StartSliceNodeInteraction(unsigned int parameters);
  vtkMRMLDisplayNode* GetDefaultMarkupsDisplayNode();
  void CopyBasicDisplayProperties(vtkMRMLDisplayNode* sourceDisplayNode, vtkMRMLDisplayNode* targetDisplayNode);
  vtkMRMLSliceNode* SliceNode;
  vtkMRMLSliceCompositeNode* SliceCompositeNode;
  void SetPlaceMode();
protected:
  vtkSlicerMeasureLogicAbstract();
  virtual ~vtkSlicerMeasureLogicAbstract();

  virtual void SetMRMLSceneInternal(vtkMRMLScene* newScene);
  /// Register MRML Node classes to Scene. Gets called automatically when the MRMLScene is attached to this logic class.
  virtual void RegisterNodes();
  virtual void UpdateFromMRMLScene();
  
  virtual void OnMRMLSceneNodeAdded(vtkMRMLNode* node);
  virtual void OnMRMLSceneNodeRemoved(vtkMRMLNode* node);
private:

  vtkSlicerMeasureLogicAbstract(const vtkSlicerMeasureLogicAbstract&); // Not implemented
  void operator=(const vtkSlicerMeasureLogicAbstract&); // Not implemented
};

#endif
