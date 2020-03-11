#include "provided.h"
#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include <queue>

using namespace std;

struct edge
{
	std::size_t dest;
	StreetSegment seg;
	struct edge *link;
	edge(std::size_t s, StreetSegment streetseg)
	{
		this->link = NULL;
		this->dest = s;
		this->seg = streetseg;
	}
};

std::size_t gethash(const GeoCoord& g)
{
    return std::hash<string>()(g.latitudeText + g.longitudeText);
}

class StreetMapImpl
{
public:
	StreetMapImpl *next;
	std::size_t name;
	StreetSegment seg;
	edge *adj;
    
	StreetMapImpl();
	StreetMapImpl(std::size_t, StreetSegment);
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
	void insert_node(std::size_t, StreetSegment);
	void insert_edge(std::size_t, std::size_t, StreetSegment);
	void display();
	void reverse(StreetSegment&);
	StreetMapImpl* find(std::size_t);
	bool isEdge(std::size_t, std::size_t);
	
};
StreetMapImpl *start;

StreetMapImpl::StreetMapImpl()
{
	
}

StreetMapImpl::StreetMapImpl(std::size_t node_name, StreetSegment streetseg)
{
	this->name = node_name;
	this->seg = streetseg;
	this->next = NULL;
	this->adj = NULL;
}

StreetMapImpl::~StreetMapImpl()
{
}

void StreetMapImpl::reverse(StreetSegment& temp){
	GeoCoord tempGEO;
	tempGEO = temp.start;
	temp.start = temp.end;
	temp.end = tempGEO;
}

void StreetMapImpl::insert_node(std::size_t node_name, StreetSegment streetSeg){
	StreetMapImpl *tmp, *ptr;
	tmp = new StreetMapImpl(node_name, streetSeg);
	
	if (start == NULL)
	{
		start = tmp;
		return;
	}
	ptr = start;
	while (ptr->next != NULL)
		ptr = ptr->next;
	ptr->next = tmp;
}

bool StreetMapImpl::isEdge(std::size_t u, std::size_t v){
	StreetMapImpl *locu, *locv;
	edge *ptr;
	locu = find(u);
	locv = find(v);
	if (!locu->adj)
		return false;
	if (locu->adj->dest == v)
	{
		return true;
	}
	else{
		ptr = locu->adj;
		if (!ptr->link)
			return false;
		while (ptr->link != NULL)
		{
			if (ptr->link->dest == v)
				return true;
			ptr = ptr->link;
		}
		return false;
	}
}

void StreetMapImpl::insert_edge(std::size_t u, std::size_t v, StreetSegment streetSeg){
	StreetMapImpl *locu, *locv;
	edge *ptr, *tmp;
	locu = find(u);
	locv = find(v);

	if (locu == NULL)
	{
		cout << "Source node not present, first insert node" << u << endl;
		return;
	}
	if (locv == NULL)
	{
		cout << "Destination node not present, first insert node" << v << endl;
		return;
	}
	tmp = new edge(v, streetSeg);
	


	if (locu->adj == NULL)  
	{
		locu->adj = tmp;
		return;
	}
	ptr = locu->adj;
	while (ptr->link != NULL)
		ptr = ptr->link;
	ptr->link = tmp;
}

StreetMapImpl* StreetMapImpl::find(std::size_t item){
	StreetMapImpl *ptr, *loc;
	ptr = start;
	while (ptr != NULL)
	{
		if (item == ptr->name)
		{
			loc = ptr;
			return loc;
		}
		else
			ptr = ptr->next;
	}
	loc = NULL;
	return loc;
}

void StreetMapImpl::display(){
	StreetMapImpl *ptr;
	edge *q;

	ptr = start;
	while (ptr != NULL)
	{
		cout << ptr->seg.start.latitudeText << ":" << ptr->name << " ->";
		
		q = ptr->adj;
		while (q != NULL)
		{
			cout <<" "<< q->dest;
			q = q->link;
		}
		cout << endl;
		ptr = ptr->next;
	}
}

bool StreetMapImpl::load(string mapFile)
{
	start = NULL;
	string line;
	ifstream myfile(mapFile);
	bool _flag_address = true;
	bool _flag_line_seg_num = false;
	bool _flag_data = false;
	
	if (myfile.is_open())
	{
		int _num_line_seg = 0;
		StreetSegment temp;
		
		while (getline(myfile, line))
		{
			if (_flag_address == true){
				
				temp.name = line;
				_flag_address = false;
				_flag_line_seg_num = true;
				_flag_data = false;
			}
			else if (_flag_line_seg_num == true)
			{
				_num_line_seg = std::stoi(line);
				_flag_data = true;
				_flag_line_seg_num = false;
			}
			else if(_flag_data == true)
			{
				--_num_line_seg;
				int geoindex = 0;
				string geovalue[4] = { "" };
				char* cstrptr = (char *)line.c_str();
				char* context = NULL;
				char *token = strtok_s(cstrptr, " ", &context);
				while (token != NULL) {
					geovalue[geoindex] = token;
					geoindex++;
					token = strtok_s(NULL, " ", &context);
				}
				
				temp.start = GeoCoord(geovalue[0],geovalue[1]);
				
				temp.end = GeoCoord(geovalue[2], geovalue[3]);
				this->reverse(temp);
				std::size_t hash_val_u, hash_val_v;
				
				hash_val_u = gethash(temp.start);

				if (!find(hash_val_u)){
					this->insert_node(hash_val_u, temp);
				}

				hash_val_v = gethash(temp.end);

				if (!find(hash_val_v)){
					this->insert_node(hash_val_v, temp);
				}

				if (!isEdge(hash_val_u, hash_val_v))
					this->insert_edge(hash_val_u, hash_val_v, temp);
				
				delete[] token;

				if (_num_line_seg == 0){
					_flag_address = true;
				}
			}
		}
		//display();

		
		vector<StreetSegment> segments;
		
		GeoCoord tempGeo = GeoCoord("34.0421285", "-118.4895033");
		if (this->getSegmentsThatStartWith(tempGeo, segments))
			cout << "yes" << endl;
		else
			cout << "no" << endl;
		
		
		myfile.close();
		return true;
	}
	else {
		cout << "Unable to open file";
		return false;  // Delete this line and implement this function correctly
	}
}


bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
	StreetMapImpl *loc_gc = NULL;
	loc_gc = loc_gc->find(gethash(gc));
	
	StreetMapImpl *ptr;
	edge *q;
	int cnt = 1;
	if (loc_gc){
		ptr = loc_gc;

		while (ptr != NULL && cnt > 0)
		{
			//cout << ptr->seg.start.latitudeText << ":" << ptr->seg.end.longitudeText << endl;
			q = ptr->adj;
			while (q != NULL)
			{
				cout << q->seg.end.latitudeText << ":" << q->seg.end.longitudeText << endl;
				segs.push_back(q->seg);
				q = q->link;
			}
			ptr = ptr->next;
			cnt--;
		}
		return true;
	}
	else
	    return false;  // Delete this line and implement this function correctly
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
   return m_impl->getSegmentsThatStartWith(gc, segs);
}
