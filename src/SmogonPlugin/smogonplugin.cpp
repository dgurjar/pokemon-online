#include "smogonplugin.h"
#include "pokemontab.h"
#include "pokemonteamtabs.h"
#include "../Teambuilder/engineinterface.h"
#include "../Teambuilder/Teambuilder/teamholderinterface.h"
#include "../PokemonInfo/pokemonstructs.h"
#include "../PokemonInfo/pokemoninfo.h"
#include "../Utilities/otherwidgets.h"
#include <QLabel>
#include <QPalette>
#include <QTabWidget>
#include <QtGui>

ClientPlugin* createPluginClass(MainEngineInterface *interface)
{
    return new SmogonPlugin(interface);
}

SmogonPlugin::SmogonPlugin(MainEngineInterface *interface) : interface(interface)
{
}

bool SmogonPlugin::hasConfigurationWidget() const
{
    return true;
}

QString SmogonPlugin::pluginName() const
{
    return "Get Builds from Smogon";
}

QWidget *SmogonPlugin::getConfigurationWidget()
{
    QWidget* ret = new QWidget;

    /* Widget that holds the Save and Cancel buttons */
    QWidget* actionButtons = new QWidget;
    QHBoxLayout* buttonsLayout = new QHBoxLayout;
   
    /* Given the interface, get the pokemon team */
    Team team = interface->trainerTeam()->team();
    Pokemon::gen m_gen = team.gen();

    /* Widget that holds the tabs */
    PokemonTeamTabs* tabs = new PokemonTeamTabs(team.path()); 

    /* Cancel button saves nothing */ 
    QPushButton* cancel = new QPushButton("Cancel", actionButtons);
    buttonsLayout -> addWidget(cancel);
    connect(cancel, SIGNAL(clicked()), ret, SLOT(close()));
    ret -> setAttribute(Qt::WA_DeleteOnClose, true);

    /* Save button saves all of the builds that were set */
    QPushButton* save = new QPushButton("Save", actionButtons);
    buttonsLayout -> addWidget(save);
    connect(save, SIGNAL(clicked()), tabs, SLOT(getTeam()));

    actionButtons -> setLayout(buttonsLayout);

    /* Share the UI between the tabs and the buttons */
    QVBoxLayout* pluginLayout = new QVBoxLayout;
    pluginLayout -> addWidget(tabs);
    pluginLayout -> addWidget(actionButtons);
    ret -> setLayout(pluginLayout);   
 
    /* Set the min dimensions of the plugin's window */
    ret -> setMinimumSize(500, 600);

    /* Create a scraper that will be shared by all of the tabs */
    
    /* Insert a tab for each pokemon in the party */
    for(int i = 0; i<6; i++){
        PokeTeam current_poke = team.poke(i);
        /* Don't display Missingno */
        if(current_poke.num() > 0){
            PokemonTab* currentTab = new PokemonTab(current_poke, m_gen, ret);
            tabs->addPokeTab(currentTab, PokemonInfo::Name(current_poke.num()));
        }
    }

    return ret;
}

