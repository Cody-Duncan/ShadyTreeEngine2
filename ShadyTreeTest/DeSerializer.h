#pragma once

class DeSerializer
{
public:
    DeSerializer(void);
    ~DeSerializer(void);
    void BuildArchetypes(std::string resID);
    void BuildSubtypes(std::string resID);
    void BuildPowerupTypes(std::string resID);
    void BuildLevel(std::string resID, std::vector<GameObject*>& levelObjects, Vector2& start, std::vector<Vector2>& startPositions);

};

