#include "Verlet.h"
#include <iostream>
#include "Link.h"


Verlet* Verlet::VerletInstance = nullptr;;

Verlet* Verlet::GetInstance()
{

    if (VerletInstance == nullptr) 
    {
        VerletInstance = new Verlet();
    }
    return VerletInstance;
}
Verlet::Verlet()
{
    m_gravity = { 0.f, 981.f };
    m_subStep = 8;   
    m_grid = CollisionGrid(sf::Vector2f(1000, 1000));
    m_constraintShape = sf::CircleShape(450.f);
    m_constraintShape.setFillColor(sf::Color::White);
    m_constraintShape.setPosition(500.f, 500.f);
    m_constraintShape.setOrigin(450.f, 450.f);
    m_constraintShape.setPointCount(100);
}


void Verlet::Update(float deltaTime)
{
    m_subDt = deltaTime / static_cast<float>(m_subStep);
    for (uint32_t i(m_subStep); i--;)
    {
	    ApplyForces();
        //Collide();
        //CheckCollision(deltaTime);
        GridCollisionResolution();
        ApplyConstraints();
	    UpdatePosition(m_subDt);
        UpdateLinks();
    }

}

void Verlet::UpdatePosition(float deltaTime)
{
    std::vector<Circle*>::iterator iter = m_circles.begin();

    unsigned int vecSize = m_circles.size();
    for (unsigned int i = 0; i < vecSize; i++)
    {
        m_circles[i]->Update(deltaTime);
    }
}

void Verlet::ApplyForces()
{
    std::vector<Circle*>::iterator iter = m_circles.begin();


    unsigned int vecSize = m_circles.size();
    for (unsigned int i = 0; i < vecSize; i++)
    {
        if (!m_circles[i]->m_isFixed)
        {
            m_circles[i]->Accelerate(m_gravity);
        }
    } 
}

void Verlet::ApplyConstraints()
{
    const sf::Vector2f constraintPos(m_constraintShape.getPosition());
    std::vector<Circle*>::iterator iter = m_circles.begin();

    unsigned int vecSize = m_circles.size();
    for (unsigned int i = 0; i < vecSize; i++)
    {

        sf::Vector2f dir = m_circles[i]->m_currentPos - constraintPos;
        float dist = VectorDistance(m_circles[i]->m_currentPos, constraintPos);
        if (dist > m_constraintShape.getRadius() - m_circles[i]->m_radius && !m_circles[i]->m_isFixed)
        {
            const sf::Vector2f r = dir / dist;
            m_circles[i]->m_currentPos = constraintPos + r * (m_constraintShape.getRadius() - m_circles[i]->m_radius);
        }
    }
}

void Verlet::Collide()
{

    float response_coef = 0.75f;
    unsigned int vecSize = m_circles.size();
    for (unsigned int i = 0; i < vecSize; i++)
    {
        Circle* c1 = m_circles[i];
        for (unsigned int j = i + 1; j < vecSize; j++)
        {
            Circle* c2 = m_circles[j];

            SolveCollision(c1, c2);
        }
    }
}

//void Verlet::CheckCollisions(float dt)
//{
//    const float    response_coef = 0.75f;
//    const uint64_t objects_count = circles.size();
//    // Iterate on all objects
//    for (uint64_t i{ 0 }; i < objects_count; ++i) {
//        Circle* circle1 = circles[i];
//        // Iterate on object involved in new collision pairs
//        for (uint64_t k{ i + 1 }; k < objects_count; ++k) {
//            Circle* circle2 = circles[k];
//            const sf::Vector2f v = circle1->m_currentPos - circle2->m_currentPos;
//            const float        dist2 = v.x * v.x + v.y * v.y;
//            const float        min_dist = circle1->m_radius + circle2->m_radius;
//            // Check overlapping
//            if (dist2 < min_dist * min_dist) {
//                const float        dist = sqrt(dist2);
//                const sf::Vector2f n = v / dist;
//                const float mass_ratio_1 = circle1->m_radius * circle1->m_radius / (circle1->m_radius * circle1->m_radius + circle2->m_radius * circle2->m_radius);
//                const float mass_ratio_2 = circle2->m_radius * circle2->m_radius / (circle1->m_radius * circle1->m_radius  + circle2->m_radius * circle2->m_radius);
//                const float delta = 0.5f * response_coef * (dist - min_dist);
//                // Update positions
//                if (circle1->m_isFixed)
//                {
//                    circle2->m_currentPos += n * (mass_ratio_1 * delta * 2);
//                }
//                else if (circle2->m_isFixed)
//                {
//                    circle1->m_currentPos -= n * (mass_ratio_2 * delta * 2);
//
//                }
//                else 
//                {
//                    circle1->m_currentPos -= n * (mass_ratio_2 * delta);
//                    circle2->m_currentPos += n * (mass_ratio_1 * delta);
//                }
//
//            }
//        }
//    }
//}


void Verlet::UpdateLinks()
{
    for (auto& link : m_links)
    {
        link->Update();
    }
}



float Verlet::VectorDistance(sf::Vector2f v1, sf::Vector2f v2)
{

    float dX = v2.x - v1.x;
    float dY = v2.y - v1.y;
    return std::sqrt(dX * dX + dY * dY);
}

void Verlet::GridCollisionResolution()
{

    
    int index = 0;
    unsigned int nbRow = m_grid.m_cells.size();
    unsigned int nbCol = m_grid.m_cells[0].size();

    auto cIt = m_circles.begin();
    while (cIt != m_circles.end())
    {
        
        int x = (*cIt)->m_currentPos.x / m_grid.m_cellSize;
        int y = (*cIt)->m_currentPos.y / m_grid.m_cellSize;
        //std::cout << " no " << index << " x = " << x << " y = " << y;
        if (x < (nbRow) && x >= 0 && y < (nbCol) && y >= 0 )
        {
            m_grid.m_cells[x][y]->circles.push_back((*cIt));
            ++cIt;
        }
        else if ((*cIt)->m_isLinked) {
            ++cIt;
        }
        else {

            delete(*cIt);
            cIt = m_circles.erase(cIt);

            //m_circles.remove(circles.begin(), circles.end(), val)
           // std::cout << "Out of range x = " << x << " and y = " << y << " and cIt = " <<  std::endl;
        }
        index++;
    }
    int collisionsolved = 0;








    for (int i = 0; i < nbRow; i++)
    {

        for (int j = 0; j < nbCol; j++)
        {
            //Circle* cel = new Circle();
            Cell* cel1 = m_grid.m_cells[i][j];
            if (!cel1->circles.empty())
            {

                for (int a = -1; a < 2; a++)
                {
                    if ((i + a) >= 0 && (i + a) < nbRow)
                    {
                        for (int b = -1; b < 2; b++)
                        {
                            if ((j + b) >= 0 && (j + b) < nbCol)
                            {
                                if (!m_grid.m_cells[i + a][j + b]->circles.empty())
                                {

                                    // Ajoutez des tâches à la file d'attente, par exemple :
                              
                                        // Effectuez les calculs de collision ici
                                        SolveCollisions(cel1, m_grid.m_cells[i + a][j + b]);


                                    collisionsolved++;
                                    //m_grid.m_cells[i + a][j + b]->circles.clear();
                                }
                            }
                        }
                    }
                }
                //SolveArrayCellsCollison(currentCells);
                cel1->circles.clear();
            }         
        }
    }
    // Lorsque vous avez terminé avec les tâches, arrêtez les threads


    // Attendez la fin des threads du pool

/*    for (int i = 1; i < nbRow - 1; i++)
    {
        m_grid.m_cells[i][0]->circles.clear();
        m_grid.m_cells[i][nbCol -1]->circles.clear();
    }
    for (int j = 0; j < nbCol; j++)
    {
        m_grid.m_cells[0][j]->circles.clear();
        m_grid.m_cells[nbRow -1][j]->circles.clear();

    } */   

            //Circle* cel = new Circle();
    //std::cout << "Collision solved = " << collisionsolved << std::endl;
    //for (unsigned int i = 0; i < nbRow; i++)
    //{
    //    for (unsigned int j = 0; j < nbCol; j++)
    //    {
    //        m_grid.m_cells[i][j]->circles.clear();
    //    }
    //}
}


void Verlet::SolveCollisions(Cell* cell1, Cell* cell2)
{

    for (auto& c1 : cell1->circles)
    {
        for (auto& c2 : cell2->circles)
        {
            if (c1 != c2)
                SolveCollision(c1, c2);
        }
    }

}


void Verlet::SolveArrayCellsCollison(std::vector<Cell*> cells)
{
    std::vector<Circle*> allCellCircles;
    for (auto& cel : cells)
    {
        for (auto& circle : cel->circles)
        {
            allCellCircles.push_back(circle);
        }
    }
   

    unsigned int vecSize = allCellCircles.size();
    for (unsigned int i = 0; i < vecSize; i++)
    {
        Circle* c1 = allCellCircles[i];
        for (unsigned int j = i + 1; j < vecSize; j++)
        {
            Circle* c2 = allCellCircles[j];
            if (c1 != c2)
            {
                SolveCollision(c1, c2);
            }
        }
    }
}



void Verlet::SolveCollision(Circle* c1, Circle* c2)
{
    float response_coef = 1.f;

    const sf::Vector2f collisionDir = c2->m_currentPos - c1->m_currentPos;
    const float dist2 = collisionDir.x * collisionDir.x + collisionDir.y * collisionDir.y;
    const float mindist = c1->m_radius + c2->m_radius;

    if (dist2 < mindist * mindist)
    {
        const float dist = sqrt(dist2);
        const float mass_ratio_1 = c1->m_radius / (c1->m_radius + c2->m_radius);
        const float mass_ratio_2 = c2->m_radius / (c1->m_radius + c2->m_radius);
        const float delta = 0.5f * response_coef * (mindist - dist);
        if (c2->m_isFixed && c1->m_isFixed)
        {
            return;
        }
        else if (c1->m_isFixed)
        {
            c2->m_currentPos += (collisionDir / dist) * (mass_ratio_1 * delta * 2);
        }
        else if (c2->m_isFixed)
        {
            c1->m_currentPos -= (collisionDir / dist) * (mass_ratio_2 * delta * 2);
        }

        else
        {
            c1->m_currentPos -= (collisionDir / dist) * (mass_ratio_2 * delta);
            c2->m_currentPos += (collisionDir / dist) * (mass_ratio_1 * delta);
        }

    }
}

void Verlet::Draw(sf::RenderWindow& window)
{
    window.draw(m_constraintShape);
    for (auto& circle : m_circles)
    {
        window.draw(circle->m_shape);
    }

}

void Verlet::Clear()
{

    for (auto circle : m_circles)
    {
        delete(circle);
    }
    m_circles.clear();
    
    for (auto link : m_links)
    {   
        delete(link);
    }
    m_links.clear();
    Circle::maxRadius = 0;
    Circle::nextRadius = Circle::globalRadius;
}

void Verlet::UpdateGrid()
{
    m_grid.UpdateGrid();
}



//std::mutex myMutex;
//std::condition_variable condition;
//std::queue<std::function<void()>> tasks;
//bool stop = false;
