#include "MyApplication.h"
#include "ApplicationListener.h"


std::map<std::string, MyApplication*> createMap()
{
	std::map<std::string, MyApplication*> map;
	return map;
}
std::map<std::string, MyApplication*> MyApplication::appsByName = createMap();

bool MyApplication::_keepRunning = true;
bool MyApplication::keepRunning()
{

	return _keepRunning;
}

void MyApplication::exit()
{
	_keepRunning = false;
}

const xml_node<>* MyApplication::getRootXMLNode()
{
	return _rootXMLNode;
}

MyApplication::MyApplication(const std::string& appName, const std::string& xmlFile)
{
		_root = NULL;
		_window = NULL;
		_listener = NULL;
		_defaultRessourcesPath = "myResources_d.cfg";
		_pluginsFilePath = "plugins_d.cfg";
		_appName = appName;
		
		//appsByName[_appName] = this;

		std::stringstream stringXML;
		std::ifstream infile;
		infile.open(xmlFile);

		std::string string;
		while(!infile.eof())
		{
			std::getline(infile, string);
			stringXML << string;
		}
		infile.close();

		std::cout<<stringXML.str() << std::endl;
		_xmlChar = strdup(stringXML.str().c_str());
		_docXML.parse<0>(_xmlChar);
		_rootXMLNode = _docXML.first_node("ApplicationDescription");
}

ApplicationListener* MyApplication::getAppListener()
{
	return _listener;
}

MyApplication::~MyApplication()
{
	
	delete _xmlChar;
	delete _root;
}

int MyApplication::go()
{
	int s = startUp();
	if (s < 0)
		return s;
	loadResources(_defaultRessourcesPath);
	finalizeRessourcesLoading();
	finalizeAppListener();

	return 0;
}

int MyApplication::startUp()
{
	_root = new Ogre::Root(_pluginsFilePath);
	if (!_root->showConfigDialog())
	{
		return -1;
	}
	_window = _root->initialise(true, _appName);
	
	return 0;
}

bool MyApplication::loadResources(std::string path)
{
	if (path.empty())
		return false;
	//Loading stuff
	Ogre::ConfigFile cf;
	cf.load(path);
	Ogre::ConfigFile::SectionIterator sectionIter = cf.getSectionIterator();

	Ogre::String sectionName, typeName, dataPath;
	
	while(sectionIter.hasMoreElements())
	{
		sectionName = sectionIter.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap* settings = sectionIter.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			dataPath = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(dataPath, typeName, sectionName);
		}
	}

	createColourCube();
	return true;
}

void MyApplication::createColourCube()
{
    /// Create the mesh via the MeshManager
    Ogre::MeshPtr msh = MeshManager::getSingleton().createManual("ColourCube", "General");
 
    /// Create one submesh
    SubMesh* sub = msh->createSubMesh();
 
    const float sqrt13 = 0.577350269f; /* sqrt(1/3) */
 
    /// Define the vertices (8 vertices, each consisting of 2 groups of 3 floats
    const size_t nVertices = 8;
    const size_t vbufCount = 3*2*nVertices;
    float vertices[vbufCount] = {
            -1,1,-1,        //0 position
            -sqrt13,sqrt13,-sqrt13,     //0 normal
            1.0,1.0,-1.0,         //1 position
            sqrt13,sqrt13,-sqrt13,      //1 normal
            1.0,-1.0,-1.0,        //2 position
            sqrt13,-sqrt13,-sqrt13,     //2 normal
            -1.0,-1.0,-1.0,       //3 position
            -sqrt13,-sqrt13,-sqrt13,    //3 normal
            -1.0,1.0,1.0,         //4 position
            -sqrt13,sqrt13,sqrt13,      //4 normal
            1.0,1.0,1.0,          //5 position
            sqrt13,sqrt13,sqrt13,       //5 normal
            1.0,-1.0,1.0,         //6 position
            sqrt13,-sqrt13,sqrt13,      //6 normal
            -1.0,-1.0,1.0,        //7 position
            -sqrt13,-sqrt13,sqrt13,     //7 normal
    };
 
    RenderSystem* rs = Root::getSingleton().getRenderSystem();
    RGBA colours[nVertices];
    RGBA *pColour = colours;
    // Use render system to convert colour value since colour packing varies
    rs->convertColourValue(ColourValue(0.0,1.0,0.0), pColour++); //0 colour
    rs->convertColourValue(ColourValue(0.0,1.0,0.0), pColour++); //1 colour
    rs->convertColourValue(ColourValue(0.0,1.0,0.0), pColour++); //2 colour
    rs->convertColourValue(ColourValue(0.0,1.0,0.0), pColour++); //3 colour
    rs->convertColourValue(ColourValue(0.0,1.0,0.0), pColour++); //4 colour
    rs->convertColourValue(ColourValue(0.0,1.0,0.0), pColour++); //5 colour
    rs->convertColourValue(ColourValue(0.0,1.0,0.0), pColour++); //6 colour
    rs->convertColourValue(ColourValue(0.0,1.0,0.0), pColour++); //7 colour
 
    /// Define 12 triangles (two triangles per cube face)
    /// The values in this table refer to vertices in the above table
    const size_t ibufCount = 36;
    unsigned short faces[ibufCount] = {
            0,2,3,
            0,1,2,
            1,6,2,
            1,5,6,
            4,6,5,
            4,7,6,
            0,7,4,
            0,3,7,
            0,5,1,
            0,4,5,
            2,7,3,
            2,6,7
    };
 
    /// Create vertex data structure for 8 vertices shared between submeshes
    msh->sharedVertexData = new VertexData();
    msh->sharedVertexData->vertexCount = nVertices;
 
    /// Create declaration (memory format) of vertex data
    VertexDeclaration* decl = msh->sharedVertexData->vertexDeclaration;
    size_t offset = 0;
    // 1st buffer
    decl->addElement(0, offset, VET_FLOAT3, VES_POSITION);
    offset += VertexElement::getTypeSize(VET_FLOAT3);
    decl->addElement(0, offset, VET_FLOAT3, VES_NORMAL);
    offset += VertexElement::getTypeSize(VET_FLOAT3);
    /// Allocate vertex buffer of the requested number of vertices (vertexCount) 
    /// and bytes per vertex (offset)
    HardwareVertexBufferSharedPtr vbuf = 
        HardwareBufferManager::getSingleton().createVertexBuffer(
        offset, msh->sharedVertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY);
    /// Upload the vertex data to the card
    vbuf->writeData(0, vbuf->getSizeInBytes(), vertices, true);
 
    /// Set vertex buffer binding so buffer 0 is bound to our vertex buffer
    VertexBufferBinding* bind = msh->sharedVertexData->vertexBufferBinding; 
    bind->setBinding(0, vbuf);
 
    // 2nd buffer
    offset = 0;
    decl->addElement(1, offset, VET_COLOUR, VES_DIFFUSE);
    offset += VertexElement::getTypeSize(VET_COLOUR);
    /// Allocate vertex buffer of the requested number of vertices (vertexCount) 
    /// and bytes per vertex (offset)
    vbuf = HardwareBufferManager::getSingleton().createVertexBuffer(
        offset, msh->sharedVertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY);
    /// Upload the vertex data to the card
    vbuf->writeData(0, vbuf->getSizeInBytes(), colours, true);
 
    /// Set vertex buffer binding so buffer 1 is bound to our colour buffer
    bind->setBinding(1, vbuf);
 
    /// Allocate index buffer of the requested number of vertices (ibufCount) 
    HardwareIndexBufferSharedPtr ibuf = HardwareBufferManager::getSingleton().
        createIndexBuffer(
        HardwareIndexBuffer::IT_16BIT, 
        ibufCount, 
        HardwareBuffer::HBU_STATIC_WRITE_ONLY);
 
    /// Upload the index data to the card
    ibuf->writeData(0, ibuf->getSizeInBytes(), faces, true);
 
    /// Set parameters of the submesh
    sub->useSharedVertices = true;
    sub->indexData->indexBuffer = ibuf;
    sub->indexData->indexCount = ibufCount;
    sub->indexData->indexStart = 0;
 
    /// Set bounding information (for culling)
    msh->_setBounds(AxisAlignedBox(-100,-100,-100,100,100,100));
    msh->_setBoundingSphereRadius(Math::Sqrt(3*100*100));
 
    /// Notify -Mesh object that it has been loaded
    msh->load();
}

void MyApplication::finalizeRessourcesLoading()
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void MyApplication::finalizeAppListener()
{
	_listener = new ApplicationListener(this);
	_root->addFrameListener(_listener);
}

void MyApplication::update()
{
	Ogre::WindowEventUtilities::messagePump();
	_keepRunning = _root->renderOneFrame();

}

Ogre::Root* MyApplication::getRoot()
{
	return _root;
};

Ogre::RenderWindow* MyApplication::getWindow()
{
	return _window;
}
