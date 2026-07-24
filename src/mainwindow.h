#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QTimer>
#include <QAction>
#include <QIcon>
#include <QCheckBox>
#include <QFrame>
#include <QStyle>
#include <QScreen>
#include <QApplication>
#include <QStackedWidget>
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleLoginClick();
    void finishLogin();
    void togglePasswordVisibility();
    void toggleTheme();
    void showLoginScreen();
    void showCreateAccountScreen();

private:
    // Main setup functions
    void setupUI();
    void setupGetStartedScreen();
    void setupLoginScreen();
    void setupCreateAccountScreen();
    void setupStyles();
    void setupLightStyles();
    void setupAnimations();
    void animateScreenTransition(int toScreen);
    void showError(const QString &message);

    // Helper functions - Login Screen Modular Components
    void setupLoginBackButton();
    void setupLoginTitleAndError();
    void setupLoginInputFields();
    void setupLoginMainButton();
    void setupLoginOrSeparator();
    void setupLoginSocialButtons();
    void setupLoginCreateAccountButton();

    // Helper functions - Create Account Screen
    void setupSignupBackButton();
    void setupSignupTitleAndInputs();
    void setupSignupButton();

    // Helper functions - Styling
    QString getPremiumButtonStyle() const;
    QString getSocialButtonStyle() const;
    QString getLinkButtonStyle() const;

    // Main stacked widget for screen management
    QStackedWidget *stackedWidget;

    // Get Started Screen
    QWidget *getStartedScreen;
    QPushButton *getStartedButton;

    // Login Screen
    QWidget *loginScreen;
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QLabel *errorLabel;
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QPushButton *loginButton;
    QProgressBar *loadingBar;
    QCheckBox *rememberMeCheckbox;
    QPushButton *forgotPasswordButton;

    QToolButton *togglePasswordButton;
    QToolButton *themeButton;
    QPushButton *googleLoginButton;
    QPushButton *githubLoginButton;
    QPushButton *createAccountButton;

    // Create Account Screen
    QWidget *createAccountScreen;
    QLineEdit *signupEmailInput;
    QLineEdit *signupPasswordInput;
    QLineEdit *signupConfirmPasswordInput;
    QPushButton *signupButton;
    QPushButton *backToLoginButton;

    bool isPasswordVisible;
    bool isDarkMode;
};

#endif // MAINWINDOW_H