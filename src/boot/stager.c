// Code written by Fom477
// Copyright Fom477 2026

#include <stdlib.h>

#include <stage.h>

StagerInstance *RegisterStage(StagerInstance *Instance, const int Index, const char *DisplayName)
{
    const int Size = Instance->StageCount <= Index? Index + 1: Instance->StageCount;

    Stage *NewStage     = malloc(sizeof(Stage));
    StagerInstance *New = realloc(Instance, sizeof(StagerInstance) + sizeof(Stage) * Size);

    NewStage->DisplayName   = DisplayName;
    NewStage->StageSequence = Index;
    NewStage->StepCount     = 0;

    New->StageCount    = Size;
    New->Stages[Index] = NewStage;

    return New;
}

void RegisterStep(StagerInstance *Instance, const int StageIndex, const int Index, const char *DisplayName, int (*SteppingFunction)(StagerInstance *Instance, Stage *Stage, Step *Step))
{
    const Stage *Original = Instance->Stages[StageIndex];
    const int Size        = Original->StepCount <= Index? Index + 1: Original->StepCount;

    Step *NewStep = malloc(sizeof(Step));
    Stage *New    = realloc(Instance->Stages[StageIndex], sizeof(Stage) + sizeof(Step) * Size);

    NewStep->DisplayName  = DisplayName;
    NewStep->StepSequence = Index;
    NewStep->DoStep       = SteppingFunction;

    New->StepCount    = Size;
    New->Steps[Index] = NewStep;

    Instance->Stages[StageIndex] = New;
}

int StageWork(StagerInstance *Instance)
{
    // TODO: Finish

    return 0;
}

int StepWork(StagerInstance *Instance)
{
    // TODO: Finish

    return 0;
}