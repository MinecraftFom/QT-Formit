// Code written by Fom477
// Copyright Fom477 2026

#ifndef FORMIT_GUI
#define FORMIT_GUI 1

#include <QPaintEvent>

namespace Formit::Gui
{
    class GuiComponent;

    namespace GitGuis
    {
        class GitUserDataComponent;
    }
}

class Formit::Gui::GuiComponent
{
public:
    virtual      ~GuiComponent() = default;

    virtual bool InRange(QVector2D); // Mouse location

    virtual void onClick(QVector2D); // Exact location of mouse
    virtual void OnDrag (QVector2D, QVector2D); // From, To
    virtual void OnDraw (QPaintEvent *);
};

class Formit::Gui::GitGuis::GitUserDataComponent : public GuiComponent
{
    GitUserDataComponent(); // When no user data is available, it will self-construct after login-ing to github and leave the AvatarPath empty (Where user should choose to upload their own avatars)
    GitUserDataComponent(const char *AvatarPath, const char *UserName, const char *UserLoginName);
};

#endif