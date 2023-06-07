#include "Ogre.h"
#include "OgreApplicationContext.h"

#include "OgreCameraMan.h"

Ogre::SceneNode* camNode;

class KeyHandler : public OgreBites::InputListener
{
    bool keyPressed(const OgreBites::KeyboardEvent& evt) override
    {
        if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
        {
            Ogre::Root::getSingleton().queueEndRendering();
        }
        return true;
    }

    bool mouseMoved(const OgreBites::MouseMotionEvent& evt)
    {
        const float mRotate = 0.13f;

        camNode->yaw(Ogre::Degree(-mRotate * evt.xrel), Ogre::Node::TS_WORLD);
        camNode->pitch(Ogre::Degree(-mRotate * evt.yrel), Ogre::Node::TS_LOCAL);

        return false;
    }

    /*virtual bool keyPressed(const OgreBites::KeyboardEvent& evt)
    {
        switch (evt.keysym.sym)
        {
        case OgreBites::SDLK_UP:
        //case OIS::KC_W:
            mDirection.z = -mMove;
            break;

        case OIS::KC_DOWN:
        //case OIS::KC_S:
            mDirection.z = mMove;
            break;

        case OIS::KC_LEFT:
        case OIS::KC_A:
            mDirection.x = -mMove;
            break;

        case OIS::KC_RIGHT:
        case OIS::KC_D:
            mDirection.x = mMove;
            break;

        case OIS::KC_PGDOWN:
        case OIS::KC_E:
            mDirection.y = -mMove;
            break;

        case OIS::KC_PGUP:
        case OIS::KC_Q:
            mDirection.y = mMove;
            break;
    }

        camNode->translate(mDirection * fe.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
        
        return false; 
    }*/


    /*virtual bool keyReleased(const KeyboardEvent& evt) {
        switch (ke.key)
        {
        case OIS::KC_UP:
        case OIS::KC_W:
            mDirection.z = 0;
            break;

        case OIS::KC_DOWN:
        case OIS::KC_S:
            mDirection.z = 0;
            break;

        case OIS::KC_LEFT:
        case OIS::KC_A:
            mDirection.x = 0;
            break;

        case OIS::KC_RIGHT:
        case OIS::KC_D:
            mDirection.x = 0;
            break;

        case OIS::KC_PGDOWN:
        case OIS::KC_E:
            mDirection.y = 0;
            break;

        case OIS::KC_PGUP:
        case OIS::KC_Q:
            mDirection.y = 0;
            break;

        default:
            break;
        }
        //return true;
        return false; 
    
    }*/
};

void SetupResources()
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator itSection = cf.getSectionIterator();

    Ogre::String strGroupName, strLocation, strType;
    while (itSection.hasMoreElements())
    {
        // Resource group.  "General", etc
        strGroupName = itSection.peekNextKey();

        Ogre::ConfigFile::SettingsMultiMap* mapSettings = itSection.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator itSetting = mapSettings->begin();

        for (; itSetting != mapSettings->end(); ++itSetting)
        {
            // Zip or FileSystem
            strType = itSetting->first;
            // Filepath
            strLocation = itSetting->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(strLocation, strType, strGroupName);
        }
    }
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

int main()
{
    OgreBites::ApplicationContext ctx("OgreTutorialApp");
    ctx.initApp();

    //SetupResources();


    // get a pointer to the already created root
    Ogre::Root* root = ctx.getRoot();
    Ogre::SceneManager* scnMgr = root->createSceneManager();

    // register our scene with the RTSS
    Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    

    // without light we would just get a black screen    
    Ogre::Light* light = scnMgr->createLight("MainLight");
    Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 10, 15);
    lightNode->attachObject(light);

    // also need to tell where we are
    camNode = scnMgr->getRootSceneNode()->createChildSceneNode();

    camNode->setPosition(10, 10, 10);
    camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_PARENT);

    // create the camera
    Ogre::Camera* cam = scnMgr->createCamera("myCam");
    cam->setNearClipDistance(5); // specific to this sample
    cam->setAutoAspectRatio(true);

    camNode->attachObject(cam);

    //Ogre::Viewport* vp = ctx.getRenderWindow()->addViewport(cam);
    //vp->setBackgroundColour(Ogre::ColourValue(0, 1, 0));

    //cam->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

    // and tell it to render into the main window
    ctx.getRenderWindow()->addViewport(cam);

    // finally something to render
   // Ogre::Entity* ent = scnMgr->createEntity("cottage_obj.obj");
    //Ogre::SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode();
    //node->attachObject(ent);

   /* Ogre::ManualObject* man = scnMgr->createManualObject("test");
    man->begin("Template/Red", Ogre::RenderOperation::OT_TRIANGLE_LIST);

    man->position(-20, 20, 20);
    man->normal(0, 0, 1);
    man->textureCoord(0, 0);

    man->position(-20, -20, 20);
    man->normal(0, 0, 1);
    man->textureCoord(0, 1);

    man->position(20, -20, 20);
    man->normal(0, 0, 1);
    man->textureCoord(1, 1);

    man->position(20, 20, 20);
    man->normal(0, 0, 1);
    man->textureCoord(1, 0);

    man->quad(0, 1, 2, 3);

    man->end();

    scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(man);*/
    
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -10);
    Ogre::MeshManager::getSingleton().createPlane(
        "ground", Ogre::RGN_DEFAULT,
        plane,
        1500, 1500, 20, 20,
        true,
        1, 5, 5,
        Ogre::Vector3::UNIT_Z);
    //! [planecreate]
    Ogre::Entity* groundEntity = scnMgr->createEntity("ground");
    //! [planenoshadow]
    groundEntity->setCastShadows(true);
    //! [planenoshadow]

    //! [planesetmat]
    groundEntity->setMaterialName("Template/Red");
    //! [planesetmat]

    scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
    
    //! [planecreate]

    try {
        Ogre::Entity* entity = scnMgr->createEntity("Sinbad", "Sinbad.mesh");
        scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entity);
    }
    catch (Ogre::Exception& e)
    {
        printf("test");
    }
    

   
    // register for input events
    //KeyHandler keyHandler;
    //ctx.addInputListener(&keyHandler);

    OgreBites::CameraMan cameraMan(camNode);
    //cameraMan.setCamera(camNode);
    ctx.addInputListener(&cameraMan);


    ctx.getRoot()->startRendering();
    ctx.closeApp();
}