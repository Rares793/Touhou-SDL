#include "include/movementGeometry.h"
#include <SDL3/SDL_events.h>
#include <algorithm>

SDL_FRect getRect(float x, float y, float renderWidth, float renderHeight){
    return SDL_FRect{
        x - renderWidth / 2.0f,
        y - renderHeight / 2.0f,
        renderWidth,
        renderHeight
    };
}

void drawControlPoints(SDL_Renderer* renderer, const CubicBezier& cb){
    SDL_SetRenderDrawColor(renderer, 60, 60, 180, 255);
    SDL_RenderLine(renderer, cb.A.x(), cb.A.y(), cb.C1.x(), cb.C1.y());
    SDL_RenderLine(renderer, cb.B.x(), cb.B.y(), cb.C2.x(), cb.C2.y());

    SDL_SetRenderDrawColor(renderer, 50, 200, 50, 255);
    SDL_FRect ra  = getRect(cb.A.x(),  cb.A.y(),  10, 10);
    SDL_FRect rb  = getRect(cb.B.x(),  cb.B.y(),  10, 10);
    SDL_RenderFillRect(renderer, &ra);
    SDL_RenderFillRect(renderer, &rb);

    SDL_SetRenderDrawColor(renderer, 200, 200, 50, 255);
    SDL_FRect rc1 = getRect(cb.C1.x(), cb.C1.y(), 8, 8);
    SDL_FRect rc2 = getRect(cb.C2.x(), cb.C2.y(), 8, 8);
    SDL_RenderFillRect(renderer, &rc1);
    SDL_RenderFillRect(renderer, &rc2);
}

void drawCatmullPoints(SDL_Renderer* renderer, CatmullRomSpline& crs){
    SDL_SetRenderDrawColor(renderer, 50, 200, 200, 255);
    Node<vec2>* current = crs.points.head;
    while(current){
        SDL_FRect r = getRect(current->v.x(), current->v.y(), 8, 8);
        SDL_RenderFillRect(renderer, &r);
        // line to next point
        if(current->next){
            SDL_SetRenderDrawColor(renderer, 40, 100, 100, 255);
            SDL_RenderLine(renderer,
                current->v.x(),       current->v.y(),
                current->next->v.x(), current->next->v.y()
            );
            SDL_SetRenderDrawColor(renderer, 50, 200, 200, 255);
        }
        current = current->next;
    }
}

void drawMovement(SDL_Renderer* renderer, Movement& mv, float totalTime){
    if(mv.type == MOVEMENT_TYPE::HOMING || mv.type == MOVEMENT_TYPE::IDLE)
        return;

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

    if(mv.type == MOVEMENT_TYPE::SPIRAL){
        for(int i = 0; i < 299; i++){
            vec2 p1 = mv.geometry.Spir.n_position(i * totalTime / 300.0f);
            vec2 p2 = mv.geometry.Spir.n_position((i + 1) * totalTime / 300.0f);
            SDL_RenderLine(renderer, p1.x(), p1.y(), p2.x(), p2.y());
        }
        return;
    }

    auto samplePosition = [&](float t) -> vec2 {
        switch(mv.type){
            case MOVEMENT_TYPE::CUBIC_BEZIER: return mv.geometry.CB.n_position(t);
            case MOVEMENT_TYPE::CIRCLE:       return mv.geometry.Circ.n_position(t);
            case MOVEMENT_TYPE::DASH:         return mv.geometry.D.n_position(t);
            case MOVEMENT_TYPE::CATMULL_ROLL: return mv.geometry.CRSpline.n_position(t);
            default:                          return errorVector;
        }
    };

    for(int i = 0; i < 99; i++){
        vec2 p1 = samplePosition(i / 100.0f);
        vec2 p2 = samplePosition((i + 1) / 100.0f);
        if(p1.x() == errorVector.x() && p1.y() == errorVector.y()) continue;
        if(p2.x() == errorVector.x() && p2.y() == errorVector.y()) continue;
        SDL_RenderLine(renderer, p1.x(), p1.y(), p2.x(), p2.y());
    }

    if(mv.type == MOVEMENT_TYPE::CUBIC_BEZIER)
        drawControlPoints(renderer, mv.geometry.CB);
    if(mv.type == MOVEMENT_TYPE::CATMULL_ROLL)
        drawCatmullPoints(renderer, mv.geometry.CRSpline);
}

void drawDot(SDL_Renderer* renderer, Movement& mv, float t, float totalTime){
    vec2 pos, tan;

    switch(mv.type){
        case MOVEMENT_TYPE::CUBIC_BEZIER:
            pos = mv.geometry.CB.n_position(t);
            tan = mv.geometry.CB.N_tangent(t);
            break;
        case MOVEMENT_TYPE::CIRCLE:
            pos = mv.geometry.Circ.n_position(t);
            tan = mv.geometry.Circ.N_tangent(t);
            break;
        case MOVEMENT_TYPE::DASH:
            pos = mv.geometry.D.n_position(t);
            tan = mv.geometry.D.N_tangent(t);
            break;
        case MOVEMENT_TYPE::CATMULL_ROLL:
            pos = mv.geometry.CRSpline.n_position(t);
            tan = mv.geometry.CRSpline.N_tangent(t);
            break;
        case MOVEMENT_TYPE::SPIRAL:
            pos = mv.geometry.Spir.n_position(totalTime);
            tan = mv.geometry.Spir.N_tangent(totalTime);
            break;
        default:
            return;
    }

    if(pos.x() == errorVector.x() && pos.y() == errorVector.y())
        return;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_FRect frect = getRect(pos.x(), pos.y(), 8, 8);
    SDL_RenderFillRect(renderer, &frect);

    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
    SDL_RenderLine(renderer,
        pos.x() - tan.x() * 20, pos.y() - tan.y() * 20,
        pos.x() + tan.x() * 20, pos.y() + tan.y() * 20
    );
}



int main(){
    SDL_Renderer* renderer {nullptr};
    SDL_Window*   window   {nullptr};

    SDL_SetAppMetadata("Touhou Wannabe", "0.1", "TH");

    if(!SDL_Init(SDL_INIT_VIDEO))
        SDL_Log("Couldn't initialise SDL_Init, %s", SDL_GetError());

    if(!SDL_CreateWindowAndRenderer("Touhou", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN, &window, &renderer))
        SDL_Log("Couldn't initialise window/renderer, %s", SDL_GetError());

    SDL_SetRenderLogicalPresentation(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    const int mvCount = 3;
    MovementState states[mvCount];

    // movement 0 — cubic bezier
    CubicBezier path(vec2(100, 100), vec2(500, 400), vec2(200, 50), vec2(400, 300));

    states[0].mv.set(path);
    
    // movement 1 — catmull rom
    states[1].mv.setCatmullRomSpline();
    states[1].mv.geometry.CRSpline.points.push(vec2(450,  400));  // phantom
    states[1].mv.geometry.CRSpline.points.push(vec2(500,  400));  // start
    states[1].mv.geometry.CRSpline.points.push(vec2(550, 200));
    states[1].mv.geometry.CRSpline.points.push(vec2(720, 300));
    states[1].mv.geometry.CRSpline.points.push(vec2(980, 150));
    states[1].mv.geometry.CRSpline.points.push(vec2(1080, 400));  // end
    states[1].mv.geometry.CRSpline.points.push(vec2(1080, 400));  // phantom

    // movement 2 — circle
    Circle circ(vec2(1080, 400), 100.0f);
    states[2].mv.set(circ);

    float  speed   = 0.3f;
    bool   running = true;
    double lastTime = SDL_GetTicks();

    while(running){
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_EVENT_QUIT)
                running = false;
            if(e.type == SDL_EVENT_KEY_DOWN){
                switch(e.key.key){
                    case SDLK_ESCAPE: running = false; break;
                    case SDLK_R:
                        for(int i = 0; i < mvCount; i++){
                            states[i].t         = 0.0f;
                            states[i].totalTime = 0.0f;
                        }
                        break;
                    case SDLK_UP:   speed = std::min(speed + 0.05f, 2.0f);  break;
                    case SDLK_DOWN: speed = std::max(speed - 0.05f, 0.05f); break;
                    default: break;
                }
            }
        }

        double currentTime = SDL_GetTicks();
        float  dt          = (currentTime - lastTime) / 1000.0f;
        lastTime           = currentTime;
        if(dt < 0.0001f) dt = 0.0001f;
        if(dt > 0.0333f) dt = 0.0333f;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for(int i = 0; i < mvCount; i++){
            states[i].t         += speed * dt;
            states[i].totalTime += dt;
            if(states[i].t >= 1.0f) states[i].t = 0.0f;

            drawMovement(renderer, states[i].mv, states[i].totalTime);
            drawDot(renderer, states[i].mv, states[i].t, states[i].totalTime);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}