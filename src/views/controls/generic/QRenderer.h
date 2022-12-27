//
// Created by tomas on 2.8.22.
//

#pragma once
#include "renderer/Scene.h"
#include "renderer/input/InputHandler.h"
#include "renderer/LightingModel.h"
#include "renderer/impl/PostProcessEffect.h"

#include "renderer/Renderer.h"
#include <memory>

#include <QOpenGLContext>
#include <QOpenGLWidget>
#include <QWindow>
#include <QBasicTimer>
#include <QElapsedTimer>
#include <QResizeEvent>




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

    private:
		std::shared_ptr<Renderer> renderer;
		std::shared_ptr<InputHandler> input;
		double frameTime = 0;

		static QRenderer *getProcAddressHelper;
        static ProcAddress getProcAddress(const char *name);

        void connectInput();

	};

} // namespace MapGenerator::Renderer
