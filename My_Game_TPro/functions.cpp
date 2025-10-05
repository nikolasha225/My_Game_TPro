#include "functions.h"

json JSONSettings = json::parse(std::ifstream("./config/settings.json"));
json JSONScore = json::parse(std::ifstream("./config/score.json"));