#ifndef ANDROIDGLINVESTIGATIONS_RENDERER_H
#define ANDROIDGLINVESTIGATIONS_RENDERER_H

#include <EGL/egl.h>
#include <memory>

#include "Model.h"
#include "Shader.h"
#include "Game.h"

struct android_app;

class Renderer {
public:
    /*!
     * @param pApp the android_app this Renderer belongs to, needed to configure GL
     */
    inline Renderer(android_app *pApp) :
            app_(pApp),
            display_(EGL_NO_DISPLAY),
            surface_(EGL_NO_SURFACE),
            context_(EGL_NO_CONTEXT),
            width_(0),
            height_(0),
            shaderNeedsNewProjectionMatrix_(true),
            game_ (Game(pApp)) {
        initRenderer();
    }

    virtual ~Renderer();

    /*!
     * Handles input from the android_app.
     *
     * Note: this will clear the input queue
     */
    void handleInput();

    /*!
     * Renders all the models in the renderer
     */
    void render();

private:
    /*!
     * Performs necessary OpenGL initialization. Customize this if you want to change your EGL
     * context or application-wide settings.
     */
    void initRenderer();

    /*!
     * @brief we have to check every frame to see if the framebuffer has changed in size. If it has,
     * update the viewport accordingly
     */
    void updateRenderArea();

    /*!
     * Creates the models for this sample. You'd likely load a scene configuration from a file or
     * use some other setup logic in your full game.
     */
    void createModels();

    Model makeTextureModel(std::size_t x, std::size_t y);
    float getTableCellX(std::size_t x, std::size_t y);
    float getTableCellY(std::size_t x, std::size_t y);
    float getTableCellWidth(std::size_t x, std::size_t y);
    float getTableCellHeight(std::size_t x, std::size_t y);
    bool tableCellContains(std::size_t x_ind, std::size_t y_ind, float x, float y);

    float toGlCoordX(float y);
    float toGlCoordY(float y);

    android_app *app_;
    EGLDisplay display_;
    EGLSurface surface_;
    EGLContext context_;
    EGLint width_;
    EGLint height_;

    bool shaderNeedsNewProjectionMatrix_;

    std::shared_ptr<TextureAsset> tic_texture;
    std::shared_ptr<TextureAsset> tac_texture;
    std::shared_ptr<TextureAsset> toe_texture;
    std::unique_ptr<Shader> texture_shader_;
    std::vector<Model> models_;

    Game game_;
};

#endif //ANDROIDGLINVESTIGATIONS_RENDERER_H