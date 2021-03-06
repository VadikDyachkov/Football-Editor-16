#include "GroupStage/matchingroup.h"

MatchInGroup::MatchInGroup( TeamInGroup &firstTeam, TeamInGroup &secondTeam ): Match( firstTeam, secondTeam ),
    firstTeamInGroup( &firstTeam ), secondTeamInGroup( &secondTeam ) {}

void MatchInGroup::setResult( int firstTeamGoals, int secondTeamGoals )
{
    if( isPlayed() )
        clear(); // pick previously acquired points
    firstTeam.setGoals( firstTeamGoals, secondTeamGoals );
    secondTeam.setGoals( secondTeamGoals, firstTeamGoals );
    updatePoints( firstTeamGoals, secondTeamGoals );
    updateResult();
}

void MatchInGroup::clear()
{
    pickPointsBack( firstTeam.getGoalsFor(), secondTeam.getGoalsFor() );
    firstTeam.clear();
    secondTeam.clear();
    result = "-:-";
}


void MatchInGroup::updatePoints( int fTeamGoals, int sTeamGoals )
{
    firstTeamInGroup->increaseGoals( fTeamGoals, sTeamGoals );
    secondTeamInGroup->increaseGoals( sTeamGoals, fTeamGoals );
    if( fTeamGoals > sTeamGoals ) {
        firstTeamInGroup->win();
        secondTeamInGroup->lose();
    }
    if( fTeamGoals < sTeamGoals ) {
        firstTeamInGroup->lose();
        secondTeamInGroup->win();
    }
    if( fTeamGoals == sTeamGoals ) {
        firstTeamInGroup->draw();
        secondTeamInGroup->draw();
    }
}

void MatchInGroup::pickPointsBack( int fTeamGoals, int sTeamGoals )
{
    firstTeamInGroup->decreaseGoals( fTeamGoals, sTeamGoals );
    secondTeamInGroup->decreaseGoals( sTeamGoals, fTeamGoals );
    if( fTeamGoals > sTeamGoals ) {
        firstTeamInGroup->unWin();
        secondTeamInGroup->unLose();
    }
    if( fTeamGoals < sTeamGoals ) {
        firstTeamInGroup->unLose();
        secondTeamInGroup->unWin();
    }
    if( fTeamGoals == sTeamGoals ) {
        firstTeamInGroup->unDraw();
        secondTeamInGroup->unDraw();
    }
}
