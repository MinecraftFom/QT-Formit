// Code written by Fom477
// Copyright Fom477 2026

#ifndef FORMIT_STAGE
#define FORMIT_STAGE 1

typedef struct StagerInstance StagerInstance;

typedef struct Stage          Stage;
typedef struct Step           Step;

struct StagerInstance
{
    int    CurrentStageIndex, CurrentStepIndex;
    int    StageCount;

    Stage *Stages[];
};

struct Stage
{
    int         StageSequence;

    int         StepCount;
    const char *DisplayName;

    Step       *Steps[];
};

struct Step
{
    int         StepSequence;
    const char *DisplayName;

    int       (*DoStep)(StagerInstance *Instance, Stage *Stage, Step *Step);
};

#ifdef __cplusplus
extern "C" StagerInstance *RegisterStage(StagerInstance *Instance, int Index, const char *DisplayName);
extern "C" void            RegisterStep (StagerInstance *Instance, int StageIndex, int Index, const char *DisplayName, int (*SteppingFunction)(StagerInstance *Instance, Stage *Stage, Step *Step));
extern "C" int             StageWork    (StagerInstance *Instance); // Steps a stage
extern "C" int             StepWork     (StagerInstance *Instance); // Steps a step in current stage
#else
StagerInstance *RegisterStage(StagerInstance *Instance, int Index, const char *DisplayName);
void            RegisterStep (StagerInstance *Instance, int StageIndex, int Index, const char *DisplayName, int (*SteppingFunction)(StagerInstance *Instance, Stage *Stage, Step *Step));
int             StageWork    (StagerInstance *Instance); // Steps a stage
int             StepWork     (StagerInstance *Instance); // Steps a step in current stage
#endif

#endif