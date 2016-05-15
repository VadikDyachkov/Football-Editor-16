#include "application.h"

void Application::showPlayoffMenu()
{
    cout << endl
         << "Playoff of " << comp->getTitle() << ":" << endl
         << "1. Show table" << endl
         << "2. Show matches" << endl
         << "3. Enter the results of the matches" << endl
         << "4. Simulate the results of the matches" << endl
         << "5. Next round" << endl
         << "9. Back to groups menu" << endl
         << "0. Exit" << endl
         << ">>> ";
    string badStr;
    int num;
    cin >> num;
    if( cin.good() )
    {
        switch( num )
        {
        case 0:
            break;
        case 1:
            showPlayoffTable(); showPlayoffMenu(); break;
        case 2:
            showPlayoffMatches(); showPlayoffMenu(); break;
        case 3:
            setResultsOfPlayoffMatches(); showPlayoffMenu(); break;
        case 4:
            simulateResultsOfPlayoffMatches(); showPlayoffMenu(); break;
        case 5:
            createNewRound(); showPlayoffMenu(); break;
        case 9:
            launchCompetitionMenu(); break;
        default:
            cout << "Error! Invalid number." << endl;
            cin.clear();
            getline(cin, badStr);
            showPlayoffMenu(); break;
        }
    }
    else
    {
        cout << "Error! Input a number." << endl;
        cin.clear();
        getline(cin, badStr);
        showPlayoffMenu();
        cout << endl;
    }
}

void Application::createPlayoffPairs()
{
    try {
        vector<int> teamIDs;
        for( Team &team: comp->getGroupStage().getWinners() )
            cout << "(" << team.getId() << ") " << team << endl;
        int teamID;
        int pairID = 0;
        for( size_t i = 0; i < comp->getGroupStage().getWinners().size() / 2; i++ ) {
            cout << "Enter 2 team's ID for " << ++pairID << " playoff pair: " << endl << ">>> ";
            for( size_t j = 0; j < 2; j++) {
                cin >> teamID;
                teamIDs.push_back(teamID);
            }
        }
        try {
            comp->startPlayOffStage(teamIDs);
        }
        catch( WrongID& e ) {
            cout << "#" << e.getWrongID() << " " << e.what() << endl
                 << "Input interrupted. Create pairs again." << endl;
            launchCompetitionMenu();
            return;
        }
    }
    catch( GroupStageIsNotOver& e ) {
        cout << e.what() << endl;
        return;
    }
    cout << "Pairs are created" << endl;
    showPlayoffMenu();
}

void Application::showPlayoffTable()
{
    try {
        cout << endl << comp->getPlayoffStage();
    }
    catch( GroupStageIsNotOver& e ) {
        cout << e.what() << endl;
    }
}

void Application::showPlayoffMatches()
{
    for( Match &match: comp->getPlayoffStage().getMatches() )
        cout << match << endl;
}

void Application::setResultsOfPlayoffMatches()
{
    try {
        cout << endl << "1/" << comp->getPlayoffStage().getCurrentRound() << endl;
        for( MatchInPlayoff &match: comp->getPlayoffStage().getMatches() ) {
            try {
                cout << match << ": ";
                cin >> match;
            }
            catch( InputPenaltryScore& e ) {
                cout << e.what() << ": ";
                int penaltyOfFirstTeam, penaltyOfSecondTeam;
                char separator;
                cin >> penaltyOfFirstTeam >> separator >> penaltyOfSecondTeam;
                try {
                    match.setPenaltyScore( penaltyOfFirstTeam, penaltyOfSecondTeam );
                }
                catch( WrongPenaltyScore& e ) {
                    cout << e.what() << endl
                         << "Input interrupted." << endl;
                    return;
                }
            }
        }
    }
    catch( GroupsAreNotCreated& e ) {
           cout << "There are no matches" << endl
                << e.what() << endl;
    }
}


void Application::simulateResultsOfPlayoffMatches()
{
    for( Match &match: comp->getPlayoffStage().getMatches() )
        match.simulate();
    showPlayoffMatches();
}

void Application::createNewRound()
{
    try {
        comp->getPlayoffStage().createNewMatches();
        showPlayoffMatches();
    }
    catch( CompetitionIsOver& e ) {
           cout << e.what() << endl;
    }
}