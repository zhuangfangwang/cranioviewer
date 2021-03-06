#include "Intersector.h"

Intersector::Intersector()
{
    mapper = nullptr;
    actor = nullptr;
    renderer = nullptr;
    transform_filter = nullptr;
    cutter = nullptr;
    stripper = nullptr;
    mesh = nullptr;
    image_slice = nullptr;
}

Intersector::~Intersector()
{
    if (actor)
    {
        renderer->RemoveActor(actor);
        std::cout<<"remove intersection actor\n";
    }
}

void Intersector::setImageSlice(ImageSlice *slice)
{
    image_slice = slice;

    renderer = image_slice->getRenderer();
}

void Intersector::setMesh(Mesh *cur_mesh)
{
    mesh = cur_mesh;
}

void Intersector::setRenderer(vtkSmartPointer<vtkRenderer> mainWin_renderer)
{
    renderer = mainWin_renderer;
}

void Intersector::setCutter()
{
    transform_filter = vtkSmartPointer< vtkTransformPolyDataFilter >::New();
    transform_filter->SetInputData(mesh->getMeshData());
    transform_filter->SetTransform(mesh->getTransform());

    cutter = vtkSmartPointer< vtkCutter >::New();
    cutter->SetInputConnection(transform_filter->GetOutputPort());
    cutter->SetCutFunction(image_slice->getSliceMapper()->GetSlicePlane());

    stripper = vtkSmartPointer< vtkStripper >::New();
    stripper->SetInputConnection(cutter->GetOutputPort());
    stripper->Update();

    mapper = vtkSmartPointer< vtkPolyDataMapper >::New();
    mapper->SetInputConnection(stripper->GetOutputPort());

    actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetDiffuseColor(0.9, 0.447, 0.0745);

    renderer->AddActor(actor);
}

void Intersector::getWorldCoord(double x, double y, double coord[3])
{
    // x y is actor coord
    vtkSmartPointer<vtkWorldPointPicker> picker = vtkSmartPointer<vtkWorldPointPicker>::New();
    picker->AddPickList(actor);
    picker->PickFromListOn();
    picker->Pick(x, y, 0.0, renderer);
    

    double pos[3];
    picker->GetPickPosition(pos);
    //std::cout<<"picked position: "<<pos[0]<<"\t"<<pos[1]<<"\t"<<pos[2]<<"\n";

    if (coord)
    {
        coord[0] = pos[0];
        coord[1] = pos[1];
        coord[2] = pos[2];
    }

    ////Create a sphere
    //vtkSmartPointer<vtkSphereSource> sphereSource =
    //    vtkSmartPointer<vtkSphereSource>::New();
    //sphereSource->SetCenter(pos[0], pos[1], pos[2]);
    //sphereSource->SetRadius(1.0);

    ////Create a mapper and actor
    //vtkSmartPointer<vtkPolyDataMapper> pick_mapper =
    //    vtkSmartPointer<vtkPolyDataMapper>::New();
    //pick_mapper->SetInputConnection(sphereSource->GetOutputPort());

    //vtkSmartPointer<vtkActor> pick_actor =
    //    vtkSmartPointer<vtkActor>::New();
    //pick_actor->SetMapper(pick_mapper);
    //pick_actor->GetProperty()->SetDiffuseColor(0.0, 1.0, 1.0);

    //renderer->AddActor(pick_actor);
}