#include "mainwindow.h"
#include "gradientlabel.h"
#include "revealoverlay.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isPasswordVisible(false), isDarkMode(true)
{
    this->setFixedSize(400, 700);
    this->setWindowTitle("Premium Login UI");
    this->setWindowIcon(QIcon(":/new/assets/login.png"));

    // Center window with screen
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - 400) / 2;
    int y = (screenGeometry.height() - 700) / 2;
    this->move(x, y);

    // Setup stacked widget for multiple screens
    stackedWidget = new QStackedWidget(this);
    this->setCentralWidget(stackedWidget);

    setupGetStartedScreen();
    setupLoginScreen();
    setupCreateAccountScreen();

    // Add screens ke stacked widget
    stackedWidget->addWidget(getStartedScreen);      // Index 0
    stackedWidget->addWidget(loginScreen);           // Index 1
    stackedWidget->addWidget(createAccountScreen);   // Index 2

    // Show Get Started screen
    stackedWidget->setCurrentIndex(0);

    setupStyles();
    setupAnimations();
}

MainWindow::~MainWindow() {}

void MainWindow::setupGetStartedScreen()
{
    getStartedScreen = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(getStartedScreen);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);
    layout->setContentsMargins(40, 40, 40, 40);

    // Logo/Title
    GradientLabel *logoLabel = new GradientLabel("Premium Login", getStartedScreen);
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setStyleSheet("font-size: 36px; font-weight: 800;");

    QLabel *subtitleLabel = new QLabel("Experience Premium Authentication", getStartedScreen);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("font-size: 14px; color: #7C7F93; margin-top: 10px;");

    // Decorative spacing
    QWidget *spacer = new QWidget(getStartedScreen);
    spacer->setMinimumHeight(100);

    // Get Started Button
    getStartedButton = new QPushButton("Get Started", getStartedScreen);
    getStartedButton->setMinimumHeight(50);
    getStartedButton->setCursor(Qt::PointingHandCursor);

    getStartedButton->setCursor(Qt::PointingHandCursor);
    connect(getStartedButton, &QPushButton::clicked, this, &MainWindow::showLoginScreen);

    QString premiumButtonStyle = R"(
        QPushButton {
            background-color: #7287FD;
            color: #FFFFFF;
            border-top: 1px solid #B4BEFE;
            border-left: 1px solid #7287FD;
            border-right: 1px solid #7287FD;
            border-bottom: 3px solid #1E66F5;
            border-radius: 12px;
            font-size: 16px;
            font-weight: 800;
        }
        QPushButton:hover {
            background-color: #89B4FA;
            border-top: 1px solid #DCE0E8;
            border-bottom: 3px solid #7287FD;
        }
        QPushButton:pressed {
            background-color: #1E66F5;
            border-top: 1px solid #1E66F5;
            border-bottom: 1px solid #1E66F5;
            padding-top: 4px;
        }
    )";
    getStartedButton->setStyleSheet(premiumButtonStyle);

    connect(getStartedButton, &QPushButton::clicked, this, &MainWindow::showLoginScreen);

    layout->addWidget(logoLabel);
    layout->addWidget(subtitleLabel);
    layout->addWidget(spacer);
    layout->addWidget(getStartedButton);
}

void MainWindow::setupLoginScreen()
{
    loginScreen = new QWidget(this);
    centralWidget = loginScreen;
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(40, 30, 40, 40);

    // Back Button
    QHBoxLayout *headerLayout = new QHBoxLayout();
    QPushButton *backButton = new QPushButton(this);

    backButton->setIcon(QIcon(":/new/assets/back.png"));
    backButton->setIconSize(QSize(24, 24));
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setStyleSheet("QPushButton { background: transparent; border: none; }");
    backButton->setMaximumWidth(40);

    connect(backButton, &QPushButton::clicked, this, [this]() {
        animateScreenTransition(0);
    });

    headerLayout->addWidget(backButton);
    headerLayout->addStretch();
    mainLayout->addLayout(headerLayout);

    titleLabel = new GradientLabel("User Login", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: 800;");

    errorLabel = new QLabel("", this);
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->hide();

    // Username Input
    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Email or Username");
    usernameInput->setMinimumHeight(45);

    // Password Input with toggle
    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Password");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setMinimumHeight(45);

    togglePasswordButton = new QToolButton(passwordInput);
    togglePasswordButton->setIcon(QIcon(":/new/assets/hide.png"));
    togglePasswordButton->setIconSize(QSize(20, 20));
    togglePasswordButton->setCursor(Qt::PointingHandCursor);

    QHBoxLayout *lineEditLayout = new QHBoxLayout(passwordInput);
    lineEditLayout->addStretch();
    lineEditLayout->addWidget(togglePasswordButton);
    lineEditLayout->setContentsMargins(0, 0, 10, 0);
    passwordInput->setLayout(lineEditLayout);
    passwordInput->setTextMargins(0, 0, 40, 0);
    connect(togglePasswordButton, &QToolButton::clicked, this, &MainWindow::togglePasswordVisibility);

    // Remember Me & Forgot Password
    QHBoxLayout *optionsLayout = new QHBoxLayout();
    rememberMeCheckbox = new QCheckBox("Remember me", this);
    rememberMeCheckbox->setCursor(Qt::PointingHandCursor);

    forgotPasswordButton = new QPushButton("Forgot password?", this);
    forgotPasswordButton->setObjectName("linkButton");
    forgotPasswordButton->setCursor(Qt::PointingHandCursor);

    optionsLayout->addWidget(rememberMeCheckbox);
    optionsLayout->addStretch();
    optionsLayout->addWidget(forgotPasswordButton);

    // Login Button
    loginButton = new QPushButton("Sign In", this);
    loginButton->setMinimumHeight(50);
    loginButton->setCursor(Qt::PointingHandCursor);

    QString premiumButtonStyle = R"(
        QPushButton {
            background-color: #7287FD;
            color: #FFFFFF;
            border-top: 1px solid #B4BEFE;
            border-left: 1px solid #7287FD;
            border-right: 1px solid #7287FD;
            border-bottom: 3px solid #1E66F5;
            border-radius: 12px;
            font-size: 16px;
            font-weight: 800;
        }
        QPushButton:hover {
            background-color: #89B4FA;
            border-top: 1px solid #DCE0E8;
            border-bottom: 3px solid #7287FD;
        }
        QPushButton:pressed {
            background-color: #1E66F5;
            border-top: 1px solid #1E66F5;
            border-bottom: 1px solid #1E66F5;
            padding-top: 4px;
        }
    )";

    loginButton->setStyleSheet(premiumButtonStyle);

    connect(loginButton, &QPushButton::clicked, this, &MainWindow::handleLoginClick);

    loadingBar = new QProgressBar(this);
    loadingBar->setRange(0, 0);
    loadingBar->setTextVisible(false);
    loadingBar->setFixedHeight(3);
    loadingBar->hide();

    // OR Separator
    QHBoxLayout *orLayout = new QHBoxLayout();
    QFrame *leftLine = new QFrame();
    leftLine->setFrameShape(QFrame::HLine);
    leftLine->setStyleSheet("border-top: 1px solid #45475A;");

    QLabel *orLabel = new QLabel("OR");
    orLabel->setAlignment(Qt::AlignCenter);
    orLabel->setFixedWidth(30);

    QFrame *rightLine = new QFrame();
    rightLine->setFrameShape(QFrame::HLine);
    rightLine->setStyleSheet("border-top: 1px solid #45475A;");

    orLayout->addWidget(leftLine, 1);
    orLayout->addWidget(orLabel, 0, Qt::AlignCenter);
    orLayout->addWidget(rightLine, 1);

    // Social Login Buttons
    googleLoginButton = new QPushButton("   Continue with Google", this);
    googleLoginButton->setObjectName("socialButton");
    googleLoginButton->setMinimumHeight(45);
    googleLoginButton->setCursor(Qt::PointingHandCursor);
    googleLoginButton->setIcon(QIcon(":/new/assets/google.png"));
    googleLoginButton->setIconSize(QSize(20, 20));

    githubLoginButton = new QPushButton("  Continue with GitHub", this);
    githubLoginButton->setObjectName("socialButton");
    githubLoginButton->setMinimumHeight(45);
    githubLoginButton->setCursor(Qt::PointingHandCursor);
    githubLoginButton->setIcon(QIcon(":/new/assets/github.png"));
    githubLoginButton->setIconSize(QSize(20, 20));

    // Create Your Account Button (NEW)
    createAccountButton = new QPushButton("Create Your Account", this);
    createAccountButton->setObjectName("socialButton");
    createAccountButton->setMinimumHeight(45);
    createAccountButton->setCursor(Qt::PointingHandCursor);
    createAccountButton->setIcon(QIcon(":/new/assets/user.png"));
    createAccountButton->setIconSize(QSize(20, 20));
    connect(createAccountButton, &QPushButton::clicked, this, &MainWindow::showCreateAccountScreen);

    // Theme Button
    themeButton = new QToolButton(this);
    themeButton->setIcon(QIcon(":/new/assets/sun.png"));
    themeButton->setIconSize(QSize(24, 24));
    themeButton->setCursor(Qt::PointingHandCursor);
    themeButton->setStyleSheet("QToolButton { border: none; background: transparent; }");
    themeButton->setGeometry(340, 20, 40, 40);
    connect(themeButton, &QToolButton::clicked, this, &MainWindow::toggleTheme);

    // Add to layout
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(errorLabel);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(usernameInput);
    mainLayout->addWidget(passwordInput);
    mainLayout->addLayout(optionsLayout);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(loginButton);
    mainLayout->addWidget(loadingBar);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(orLayout);
    mainLayout->addSpacing(8);
    mainLayout->addWidget(googleLoginButton);
    mainLayout->addWidget(githubLoginButton);
    mainLayout->addWidget(createAccountButton);  // NEW
    mainLayout->addStretch();
}

void MainWindow::setupCreateAccountScreen()
{
    createAccountScreen = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(createAccountScreen);
    layout->setAlignment(Qt::AlignTop);
    layout->setSpacing(12);
    layout->setContentsMargins(40, 30, 40, 40);

    // Back button
    QHBoxLayout *headerLayout = new QHBoxLayout();
    backToLoginButton = new QPushButton(this);

    backToLoginButton->setIcon(QIcon(":/new/assets/back.png"));
    backToLoginButton->setIconSize(QSize(24, 24));
    backToLoginButton->setCursor(Qt::PointingHandCursor);
    backToLoginButton->setStyleSheet("QPushButton { background: transparent; border: none; }");
    backToLoginButton->setMaximumWidth(40);

    connect(backToLoginButton, &QPushButton::clicked, this, [this]() {
        animateScreenTransition(1);
    });

    headerLayout->addWidget(backToLoginButton);
    headerLayout->addStretch();
    layout->addLayout(headerLayout);

    // Title
    GradientLabel *signupTitle = new GradientLabel("Create Account", this);
    signupTitle->setAlignment(Qt::AlignCenter);
    signupTitle->setStyleSheet("font-size: 28px; font-weight: 800;");

    // Email Input
    signupEmailInput = new QLineEdit(this);
    signupEmailInput->setPlaceholderText("Email Address");
    signupEmailInput->setMinimumHeight(45);

    // Password Input
    signupPasswordInput = new QLineEdit(this);
    signupPasswordInput->setPlaceholderText("Password");
    signupPasswordInput->setEchoMode(QLineEdit::Password);
    signupPasswordInput->setMinimumHeight(45);

    QToolButton *toggleSignupPwdBtn = new QToolButton(signupPasswordInput);
    toggleSignupPwdBtn->setIcon(QIcon(":/new/assets/hide.png"));
    toggleSignupPwdBtn->setIconSize(QSize(20, 20));
    toggleSignupPwdBtn->setCursor(Qt::PointingHandCursor);

    QHBoxLayout *pwdLayout = new QHBoxLayout(signupPasswordInput);
    pwdLayout->addStretch();
    pwdLayout->addWidget(toggleSignupPwdBtn);
    pwdLayout->setContentsMargins(0, 0, 10, 0);
    signupPasswordInput->setLayout(pwdLayout);
    signupPasswordInput->setTextMargins(0, 0, 40, 0);

    // Confirm Password
    signupConfirmPasswordInput = new QLineEdit(this);
    signupConfirmPasswordInput->setPlaceholderText("Confirm Password");
    signupConfirmPasswordInput->setEchoMode(QLineEdit::Password);
    signupConfirmPasswordInput->setMinimumHeight(45);

    QToolButton *toggleConfirmPwdBtn = new QToolButton(signupConfirmPasswordInput);
    toggleConfirmPwdBtn->setIcon(QIcon(":/new/assets/hide.png"));
    toggleConfirmPwdBtn->setIconSize(QSize(20, 20));
    toggleConfirmPwdBtn->setCursor(Qt::PointingHandCursor);

    QHBoxLayout *confirmPwdLayout = new QHBoxLayout(signupConfirmPasswordInput);
    confirmPwdLayout->addStretch();
    confirmPwdLayout->addWidget(toggleConfirmPwdBtn);
    confirmPwdLayout->setContentsMargins(0, 0, 10, 0);
    signupConfirmPasswordInput->setLayout(confirmPwdLayout);
    signupConfirmPasswordInput->setTextMargins(0, 0, 40, 0);

    connect(toggleSignupPwdBtn, &QToolButton::clicked, this, [this, toggleSignupPwdBtn]() {
        if (signupPasswordInput->echoMode() == QLineEdit::Password) {
            signupPasswordInput->setEchoMode(QLineEdit::Normal);
            toggleSignupPwdBtn->setIcon(QIcon(":/new/assets/view.png"));
        } else {
            signupPasswordInput->setEchoMode(QLineEdit::Password);
            toggleSignupPwdBtn->setIcon(QIcon(":/new/assets/hide.png"));
        }
    });

    connect(toggleConfirmPwdBtn, &QToolButton::clicked, this, [this, toggleConfirmPwdBtn]() {
        if (signupConfirmPasswordInput->echoMode() == QLineEdit::Password) {
            signupConfirmPasswordInput->setEchoMode(QLineEdit::Normal);
            toggleConfirmPwdBtn->setIcon(QIcon(":/new/assets/view.png"));
        } else {
            signupConfirmPasswordInput->setEchoMode(QLineEdit::Password);
            toggleConfirmPwdBtn->setIcon(QIcon(":/new/assets/hide.png"));
        }
    });

    QCheckBox *termsCheckbox = new QCheckBox("I agree to the Terms & Privacy Policy", this);
    termsCheckbox->setCursor(Qt::PointingHandCursor);

    termsCheckbox->setStyleSheet("margin-top: 5px; margin-bottom: 5px;");



    // Sign Up Button
    signupButton = new QPushButton("Create Account", this);
    signupButton->setMinimumHeight(50);
    signupButton->setCursor(Qt::PointingHandCursor);
    QString premiumButtonStyle = R"(
        QPushButton {
            background-color: #7287FD;
            color: #FFFFFF;
            border-top: 1px solid #B4BEFE;
            border-left: 1px solid #7287FD;
            border-right: 1px solid #7287FD;
            border-bottom: 3px solid #1E66F5;
            border-radius: 12px;
            font-size: 16px;
            font-weight: 800;
        }
        QPushButton:hover {
            background-color: #89B4FA;
            border-top: 1px solid #DCE0E8;
            border-bottom: 3px solid #7287FD;
        }
        QPushButton:pressed {
            background-color: #1E66F5;
            border-top: 1px solid #1E66F5;
            border-bottom: 1px solid #1E66F5;
            padding-top: 4px;
        }
    )";

    signupButton->setStyleSheet(premiumButtonStyle);

    layout->addWidget(signupTitle);
    layout->addSpacing(15);
    layout->addWidget(signupEmailInput);
    layout->addWidget(signupPasswordInput);
    layout->addWidget(signupConfirmPasswordInput);
    layout->addWidget(termsCheckbox);
    layout->addSpacing(10);
    layout->addWidget(signupButton);
    layout->addStretch();
}

void MainWindow::setupStyles()
{
    // Soft Orange & Purple Dark Theme
    QString styleSheet = R"(
        QMainWindow {
            background-color: #1A1A2E;
        }
        QLabel {
            color: #E8D5F2;
        }
        QLineEdit {
            background-color: #2D2D44;
            color: #E8D5F2;
            border: 2px solid #4A4A6A;
            border-radius: 10px;
            padding: 10px 15px;
            font-size: 14px;
        }
        QLineEdit:focus {
            border: 2px solid #FF9E64;
        }
        QLineEdit[inputError="true"] {
            border: 2px solid #FF6B6B;
        }
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #FF9E64, stop:1 #D699B8);
            color: white;
            border: none;
            border-radius: 10px;
            font-size: 16px;
            font-weight: bold;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #FFB380, stop:1 #E0A8CC);
        }
        QPushButton:disabled {
            background-color: #4A4A6A;
            color: #A6ADC8;
        }
        QProgressBar {
            background-color: #2D2D44;
            border-radius: 2px;
        }
        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #FF9E64, stop:1 #D699B8);
        }
        QCheckBox {
            color: #C8B8D8;
            font-size: 13px;
        }
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border-radius: 4px;
            border: 1px solid #4A4A6A;
            background-color: #2D2D44;
        }
        QCheckBox::indicator:checked {
            background-color: #FF9E64;
            border: 1px solid #FF9E64;
        }
        QPushButton#linkButton {
            background-color: transparent;
            color: #FF9E64;
            font-size: 13px;
            font-weight: normal;
        }
        QPushButton#linkButton:hover {
            color: #FFB380;
            text-decoration: underline;
        }
        QPushButton#socialButton {
            background-color: transparent;
            color: #E8D5F2;
            border: 2px solid #4A4A6A;
            font-weight: normal;
            border-radius: 10px;
        }
        QPushButton#socialButton:hover {
            background-color: #2D2D44;
            border: 2px solid #FF9E64;
        }
    )";
    this->setStyleSheet(styleSheet);
}

void MainWindow::setupLightStyles()
{
    QString styleSheet = R"(
        QMainWindow {
            background-color: #FFF9F5;
        }
        QLabel {
            color: #5A4A6A;
        }
        QLineEdit {
            background-color: #F5E8F0;
            color: #5A4A6A;
            border: 2px solid #D4C4D8;
            border-radius: 10px;
            padding: 10px 15px;
            font-size: 14px;
        }
        QLineEdit:focus {
            border: 2px solid #FF9E64;
        }
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #FF9E64, stop:1 #D699B8);
            color: white;
            border: none;
            border-radius: 10px;
            font-size: 16px;
            font-weight: bold;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #FFB380, stop:1 #E0A8CC);
        }
        QProgressBar {
            background-color: #F5E8F0;
            border-radius: 2px;
        }
        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #FF9E64, stop:1 #D699B8);
        }
        QCheckBox {
            color: #6A5A7A;
            font-size: 13px;
        }
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border-radius: 4px;
            border: 1px solid #D4C4D8;
            background-color: #F5E8F0;
        }
        QCheckBox::indicator:checked {
            background-color: #FF9E64;
            border: 1px solid #FF9E64;
        }
        QPushButton#linkButton {
            background-color: transparent;
            color: #FF9E64;
            font-size: 13px;
        }
        QPushButton#socialButton {
            background-color: transparent;
            color: #5A4A6A;
            border: 2px solid #D4C4D8;
            font-weight: normal;
            border-radius: 10px;
        }
        QPushButton#socialButton:hover {
            background-color: #F5E8F0;
            border: 2px solid #FF9E64;
        }
    )";
    this->setStyleSheet(styleSheet);
}

void MainWindow::setupAnimations()
{
    QPropertyAnimation *fadeAnimation = new QPropertyAnimation(this, "windowOpacity");
    fadeAnimation->setDuration(600);
    fadeAnimation->setStartValue(0.0);
    fadeAnimation->setEndValue(1.0);
    fadeAnimation->setEasingCurve(QEasingCurve::InOutQuad);
    fadeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::showLoginScreen()
{
    animateScreenTransition(1);
}

void MainWindow::showCreateAccountScreen()
{
    animateScreenTransition(2);
}

void MainWindow::animateScreenTransition(int toScreen)
{
    int currentIndex = stackedWidget->currentIndex();
    if (currentIndex == toScreen) return;

    QWidget *oldPage = stackedWidget->widget(currentIndex);
    QWidget *newPage = stackedWidget->widget(toScreen);

    bool slideLeft = (toScreen > currentIndex);
    int offset = slideLeft ? stackedWidget->width() : -stackedWidget->width();


    newPage->setGeometry(offset, 0, stackedWidget->width(), stackedWidget->height());
    newPage->show();
    newPage->raise();


    QParallelAnimationGroup *animGroup = new QParallelAnimationGroup(this);

    QPropertyAnimation *animOld = new QPropertyAnimation(oldPage, "pos");
    animOld->setDuration(450);
    animOld->setStartValue(QPoint(0, 0));
    animOld->setEndValue(QPoint(-offset, 0));
    animOld->setEasingCurve(QEasingCurve::InOutQuart);

    QPropertyAnimation *animNew = new QPropertyAnimation(newPage, "pos");
    animNew->setDuration(450);
    animNew->setStartValue(QPoint(offset, 0));
    animNew->setEndValue(QPoint(0, 0));
    animNew->setEasingCurve(QEasingCurve::InOutQuart);

    animGroup->addAnimation(animOld);
    animGroup->addAnimation(animNew);


    connect(animGroup, &QParallelAnimationGroup::finished, this, [this, toScreen, oldPage, animGroup]() {
        stackedWidget->setCurrentIndex(toScreen);
        oldPage->hide();
        animGroup->deleteLater();
    });

    animGroup->start();
}

void MainWindow::showError(const QString &message)
{
    errorLabel->setText(message);
    errorLabel->setStyleSheet("color: #FF6B6B; font-size: 13px; font-weight: normal;");
    errorLabel->show();

    usernameInput->setProperty("inputError", true);
    passwordInput->setProperty("inputError", true);

    usernameInput->style()->unpolish(usernameInput);
    usernameInput->style()->polish(usernameInput);
    passwordInput->style()->unpolish(passwordInput);
    passwordInput->style()->polish(passwordInput);
}

void MainWindow::handleLoginClick()
{
    if (usernameInput->text().trimmed().isEmpty() || passwordInput->text().isEmpty()) {
        showError("Username and password are required.");
        return;
    }

    errorLabel->hide();
    usernameInput->setProperty("inputError", false);
    passwordInput->setProperty("inputError", false);
    usernameInput->style()->unpolish(usernameInput);
    usernameInput->style()->polish(usernameInput);
    passwordInput->style()->unpolish(passwordInput);
    passwordInput->style()->polish(passwordInput);

    usernameInput->setEnabled(false);
    passwordInput->setEnabled(false);
    rememberMeCheckbox->setEnabled(false);
    loginButton->setEnabled(false);
    googleLoginButton->setEnabled(false);
    githubLoginButton->setEnabled(false);
    createAccountButton->setEnabled(false);

    loginButton->setText("Signing In...");
    loadingBar->show();

    QTimer::singleShot(2500, this, &MainWindow::finishLogin);
}

void MainWindow::finishLogin()
{
    loadingBar->hide();
    loginButton->setText("Success!");
    loginButton->setStyleSheet("background-color: #6EE7B7; color: #1A1A2E;");
}

void MainWindow::togglePasswordVisibility()
{
    if (isPasswordVisible) {
        passwordInput->setEchoMode(QLineEdit::Password);
        togglePasswordButton->setIcon(QIcon(":/new/assets/hide.png"));
        isPasswordVisible = false;
    } else {
        passwordInput->setEchoMode(QLineEdit::Normal);
        togglePasswordButton->setIcon(QIcon(":/new/assets/view.png"));
        isPasswordVisible = true;
    }
}

void MainWindow::toggleTheme()
{
    if (themeButton->property("isAnimating").toBool()) {
        return;
    }

    themeButton->setProperty("isAnimating", true);

    QPixmap oldUiPixmap = this->grab();

    RevealOverlay *overlay = new RevealOverlay(this);
    overlay->resize(this->size());
    overlay->setPixmap(oldUiPixmap);
    overlay->setCenter(themeButton->geometry().center());
    overlay->show();
    overlay->raise();

    if (isDarkMode) {
        setupLightStyles();
        themeButton->setIcon(QIcon(":/new/assets/moon.png"));
        isDarkMode = false;
    } else {
        setupStyles();
        themeButton->setIcon(QIcon(":/new/assets/sun.png"));
        isDarkMode = true;
    }

    QPropertyAnimation *anim = new QPropertyAnimation(overlay, "radius");
    anim->setDuration(550);
    anim->setStartValue(0);
    anim->setEndValue(755);
    anim->setEasingCurve(QEasingCurve::InOutQuad);

    connect(anim, &QPropertyAnimation::finished, this, [this, overlay]() {
        themeButton->setProperty("isAnimating", false);
        overlay->deleteLater();
    });

    QTimer::singleShot(50, this, [anim]() {
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    });
}