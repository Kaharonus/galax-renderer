//
// Created by tomas on 2.8.22.
//

#pragma once
#include "Scene.h"
#include "input/InputHandler.h"
#include "GBuffer.h"
#include "LightingModel.h"
#include "impl/PostProcessEffect.h"

#include <chrono>
#include <iostream>
#include <memory>

#include <QOpenGLContext>
#include <QOpenGLWidget>
#include <QWindow>
#include <QBasicTimer>
#include <QElapsedTimer>




namespace Galax::Renderer {

    using ProcAddress = void (*)();


    class QRenderer : public QWindow {
        Q_OBJECT
    public:

        enum VerticalSync{
            Disabled = 0,
            Enabled = 1,
            Adaptive = -1
        };


        explicit QRenderer(std::shared_ptr<InputHandler> inputHandler,const QSurfaceFormat& format, QScreen* screen = nullptr);

        void setVerticalSync(VerticalSync sync);

        void setScene(std::shared_ptr<Scene> scene);
        void setLightingModel(std::shared_ptr<LightingModel> lightingModel);
        void addPostProcess(std::shared_ptr<IPostProcessEffect> postProcess);

        double getFrameTime() const;

        ~QRenderer();

    protected:
        virtual void initializeGL(const QSurfaceFormat &format);
        virtual void paintGL();
        void timerEvent(QTimerEvent *event);

    signals:

        void wheelEvent(QWheelEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;
        void keyReleaseEvent(QKeyEvent* event) override;

    protected:
        QScopedPointer<QOpenGLContext> context;
        VerticalSync sync;
        QScopedPointer<QBasicTimer> repaintTimer;

        bool update;

    private:
        int viewportWidth;
        int viewportHeight;

        std::shared_ptr<Scene> scene;
        std::shared_ptr<LightingModel> lightingModel;
        std::vector<std::shared_ptr<IPostProcessEffect>> postProcesses;

        std::shared_ptr<GBuffer> gBuffer;

        double frameTime = 0.0;
        static QRenderer *getProcAddressHelper;
        static ProcAddress getProcAddress(const char *name);
        std::shared_ptr<InputHandler> input;

        void connectInput();

        void resize();

    };

} // namespace MapGenerator::Renderer
