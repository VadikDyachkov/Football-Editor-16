#ifndef ROUND_H
#define ROUND_H

#include "matchinplayoff.h"

class Round
{
public:
    Round( RoundEnum round );
    void addPair( TeamInPlayoff &first, TeamInPlayoff &second );
    RoundEnum getRoundEnum() const;
    void addTeams(vector<TeamInPlayoff> &teams_ );
    vector<TeamInPlayoff> &getTeams();
    vector<MatchInPlayoff> &getMatches();
    vector<TeamInPlayoff> &getWinners();
    void updateWinners();

private:
    RoundEnum round;
    vector<TeamInPlayoff> teams;
    vector<MatchInPlayoff> matches;
    vector<TeamInPlayoff> winners;
};

#endif // ROUND_H
