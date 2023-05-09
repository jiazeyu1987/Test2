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

// Measure Logic includes 
#include "vtkSlicerMeasureLogicAbstract.h"

// MRML includes
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkIntArray.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>

// STD includes
#include <cassert>
#include "vtkMRMLDisplayableNode.h"
#include "vtkMRMLNode.h"
#include "vtkMRMLDisplayNode.h"
#include "vtkMRMLVolumeNode.h"
//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerMeasureLogicAbstract);

//----------------------------------------------------------------------------
vtkSlicerMeasureLogicAbstract::vtkSlicerMeasureLogicAbstract()
{
    this->SliceNode = nullptr;
    this->SliceCompositeNode = nullptr; 
}

//----------------------------------------------------------------------------
vtkSlicerMeasureLogicAbstract::~vtkSlicerMeasureLogicAbstract()
{
}

//----------------------------------------------------------------------------
void vtkSlicerMeasureLogicAbstract::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}


//---------------------------------------------------------------------------
std::string vtkSlicerMeasureLogicAbstract::GetSelectionNodeID()
{
    std::string selectionNodeID = std::string("");

    if (!this->GetMRMLScene())
    {
        vtkErrorMacro("GetSelectionNodeID: no scene defined!");
        return selectionNodeID;
    }

    // try the application logic first
    vtkMRMLApplicationLogic* mrmlAppLogic = this->GetMRMLApplicationLogic();
    if (mrmlAppLogic)
    {
        vtkMRMLSelectionNode* selectionNode = mrmlAppLogic->GetSelectionNode();
        if (selectionNode)
        {
            char* id = selectionNode->GetID();
            if (id)
            {
                selectionNodeID = std::string(id);
            }
        }
    }
    else
    {
        // try a default string
        selectionNodeID = std::string("vtkMRMLSelectionNodeSingleton");
        // check if it's in the scene
        if (this->GetMRMLScene()->GetNodeByID(selectionNodeID.c_str()) == nullptr)
        {
            vtkErrorMacro("GetSelectionNodeID: no selection node in scene with id " << selectionNodeID);
            // reset it
            selectionNodeID = std::string("");
        }
    } 
    return selectionNodeID;
}

//---------------------------------------------------------------------------
void vtkSlicerMeasureLogicAbstract::SetActiveListID(vtkMRMLNode* markupsNode)
{ 
    vtkMRMLSelectionNode* selectionNode = vtkMRMLSelectionNode::SafeDownCast(this->GetMRMLScene()->GetNodeByID(this->GetSelectionNodeID().c_str()));
    if (!selectionNode)
    {
        vtkErrorMacro("vtkSlicerMarkupsLogic::SetActiveListID: No selection node in the scene.");
        return;
    }

    if (markupsNode == nullptr)
    {
        // If fiducial node was placed then reset node ID and deactivate placement
        const char* activePlaceNodeClassName = selectionNode->GetActivePlaceNodeClassName();
        if (activePlaceNodeClassName && strcmp(activePlaceNodeClassName, "vtkMRMLMarkupsFiducialNode") == 0)
        {
            selectionNode->SetReferenceActivePlaceNodeID(nullptr);
            vtkSmartPointer<vtkCollection> interactionNodes = vtkSmartPointer<vtkCollection>::Take
            (this->GetMRMLScene()->GetNodesByClass("vtkMRMLInteractionNode"));
            for (int interactionNodeIndex = 0; interactionNodeIndex < interactionNodes->GetNumberOfItems(); ++interactionNodeIndex)
            {
                vtkMRMLInteractionNode* interactionNode = vtkMRMLInteractionNode::SafeDownCast(interactionNodes->GetItemAsObject(interactionNodeIndex));
                if (interactionNode->GetCurrentInteractionMode() == vtkMRMLInteractionNode::Place)
                {
                    interactionNode->SetCurrentInteractionMode(vtkMRMLInteractionNode::ViewTransform);
                }
            }
        }
        return;
    }

    // check if need to update the current type of node that's being placed
    const char* activePlaceNodeClassName = selectionNode->GetActivePlaceNodeClassName();
    if (!activePlaceNodeClassName ||
        (activePlaceNodeClassName &&
            strcmp(activePlaceNodeClassName, markupsNode->GetClassName()) != 0))
    {
        // call the set reference to make sure the event is invoked
        selectionNode->SetReferenceActivePlaceNodeClassName(markupsNode->GetClassName());
    }
    // set this markup node active if it's not already
    const char* activePlaceNodeID = selectionNode->GetActivePlaceNodeID();
    if (!activePlaceNodeID ||
        (activePlaceNodeID && strcmp(activePlaceNodeID, markupsNode->GetID()) != 0))
    {
        selectionNode->SetReferenceActivePlaceNodeID(markupsNode->GetID());
    }
}

//---------------------------------------------------------------------------
std::string vtkSlicerMeasureLogicAbstract::AddNewDisplayNodeForMarkupsNode(vtkMRMLNode* mrmlNode)
{
    std::string id;
    if (!mrmlNode || !mrmlNode->GetScene())
    {
        vtkErrorMacro("AddNewDisplayNodeForMarkupsNode: unable to add a markups display node!");
        return id;
    }

    // is there already a display node?
    vtkMRMLDisplayableNode* displayableNode = vtkMRMLDisplayableNode::SafeDownCast(mrmlNode);
    if (displayableNode && displayableNode->GetDisplayNode() != nullptr)
    {
        return displayableNode->GetDisplayNodeID();
    }

    // create the display node
    displayableNode->CreateDefaultDisplayNodes();
    vtkMRMLDisplayNode* displayNode = vtkMRMLDisplayNode::SafeDownCast(displayableNode->GetDisplayNode());
    if (!displayNode)
    {
        vtkErrorMacro("AddNewDisplayNodeForMarkupsNode: error creating new display node");
        return id;
    }

    // set it from the defaults
    this->SetDisplayNodeToDefaults(displayNode);
    vtkDebugMacro("AddNewDisplayNodeForMarkupsNode: set display node to defaults");

    // get the node id to return
    id = std::string(displayNode->GetID());

    
    return id;
}

void vtkSlicerMeasureLogicAbstract::SetPlaceMode() {
    vtkMRMLInteractionNode* interactionNode = nullptr;
    if (GetMRMLScene())
    {
        interactionNode = vtkMRMLInteractionNode::SafeDownCast(GetMRMLScene()->GetNodeByID("vtkMRMLInteractionNodeSingleton"));
    }
    interactionNode->SetCurrentInteractionMode(vtkMRMLInteractionNode::Place);
}

//---------------------------------------------------------------------------
void vtkSlicerMeasureLogicAbstract::SetDisplayNodeToDefaults(vtkMRMLDisplayNode* displayNode)
{
    if (!displayNode)
    {
        return;
    }
    if (!this->GetMRMLScene())
    { 
        return;
    }
    vtkMRMLDisplayNode* defaultNode = this->GetDefaultMarkupsDisplayNode();
    if (!defaultNode)
    {
        return;
    }
    this->CopyBasicDisplayProperties(defaultNode, displayNode);
}

//---------------------------------------------------------------------------
void vtkSlicerMeasureLogicAbstract::CopyBasicDisplayProperties(vtkMRMLDisplayNode* sourceDisplayNode,
    vtkMRMLDisplayNode* targetDisplayNode)
{
    if (!sourceDisplayNode || !targetDisplayNode)
    {
        vtkErrorMacro("vtkSlicerMarkupsLogic::CopyBasicDisplayProperties failed: invalid input nodes");
        return;
    }
    MRMLNodeModifyBlocker blocker(targetDisplayNode);
    targetDisplayNode->SetSelectedColor(sourceDisplayNode->GetSelectedColor());
    targetDisplayNode->SetColor(sourceDisplayNode->GetColor());
}

vtkMRMLDisplayNode* vtkSlicerMeasureLogicAbstract::GetDefaultMarkupsDisplayNode()
{
    if (!this->GetMRMLScene())
    {
        return nullptr;
    }
    vtkMRMLDisplayNode* defaultNode = vtkMRMLDisplayNode::SafeDownCast(
        this->GetMRMLScene()->GetDefaultNodeByClass("vtkMRMLDisplayNode"));
    if (defaultNode)
    {
        return defaultNode;
    }
    vtkSmartPointer<vtkMRMLDisplayNode> newDefaultNode =
        vtkSmartPointer<vtkMRMLDisplayNode>::Take(vtkMRMLDisplayNode::SafeDownCast(
            this->GetMRMLScene()->CreateNodeByClass("vtkMRMLDisplayNode")));
    if (!newDefaultNode)
    {
        return nullptr;
    }
    this->GetMRMLScene()->AddDefaultNode(newDefaultNode);
    return newDefaultNode;
}

//---------------------------------------------------------------------------
void vtkSlicerMeasureLogicAbstract::SetMRMLSceneInternal(vtkMRMLScene * newScene)
{
  vtkNew<vtkIntArray> events;
  events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
  events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
  events->InsertNextValue(vtkMRMLScene::EndBatchProcessEvent);
  this->SetAndObserveMRMLSceneEventsInternal(newScene, events.GetPointer());
}

//-----------------------------------------------------------------------------
void vtkSlicerMeasureLogicAbstract::RegisterNodes()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerMeasureLogicAbstract::UpdateFromMRMLScene()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerMeasureLogicAbstract
::OnMRMLSceneNodeAdded(vtkMRMLNode* node)
{
    if (!node)
    {
        return;
    }
    if (node->IsA("vtkMRMLDisplayNode"))
    {
        vtkDebugMacro("OnMRMLSceneNodeAdded: Have a markups display node");
        vtkNew<vtkIntArray> events;
        vtkUnObserveMRMLNodeMacro(node);
        vtkObserveMRMLNodeEventsMacro(node, events.GetPointer());
    }
    // a node could have been added by a node selector's create new node method,
    // but make sure that the scene is not batch processing before responding to
    // the event
    if (!node->IsA("vtkMRMLNode"))
    {
        return;
    }
    if (this->GetMRMLScene() &&
        (this->GetMRMLScene()->IsImporting() ||
            this->GetMRMLScene()->IsRestoring() ||
            this->GetMRMLScene()->IsBatchProcessing()))
    {
        return;
    }
    vtkMRMLNode* markupsNode = vtkMRMLNode::SafeDownCast(node);
    if (!markupsNode)
    {
        return;
    }

    
    // make it active for adding to via the mouse
    this->SetActiveListID(markupsNode);
}

//---------------------------------------------------------------------------
void vtkSlicerMeasureLogicAbstract
::OnMRMLSceneNodeRemoved(vtkMRMLNode* node)
{
    // remove observer
    if (!node)
    {
        return;
    }
    if (node->IsA("vtkMRMLDisplayNode"))
    {
        vtkDebugMacro("OnMRMLSceneNodeRemoved: Have a markups display node");
        vtkUnObserveMRMLNodeMacro(node);
    }
}




void vtkSlicerMeasureLogicAbstract::StartSliceNodeInteraction(unsigned int parameters)
{
    if (this->SliceNode == nullptr || this->SliceCompositeNode == nullptr)
    {
        return;
    }

    // Cache the flags on what parameters are going to be modified. Need
    // to this this outside the conditional on HotLinkedControl and LinkedControl
    this->SliceNode->SetInteractionFlags(parameters);

    // If we have hot linked controls, then we want to broadcast changes
    if ((this->SliceCompositeNode->GetHotLinkedControl() || parameters == vtkMRMLSliceNode::MultiplanarReformatFlag)
        && this->SliceCompositeNode->GetLinkedControl())
    {
        this->SliceNode->InteractingOn();
    }
}