#ifndef VIRTUAL_EVENTS_QUEUE
#define VIRTUAL_EVENTS_QUEUE


#include <vector>
#include "VirtualEvent.h"


class VirtualEventsQueue
{
	public:
		VirtualEventsQueue() = default;
		VirtualEventsQueue(const VirtualEventsQueue &other) = delete;
		VirtualEventsQueue(VirtualEventsQueue &&other) = delete;
		VirtualEventsQueue& operator=(const VirtualEventsQueue &other) = delete;
		VirtualEventsQueue& operator=(VirtualEventsQueue &&other) = delete;
		virtual ~VirtualEventsQueue() = default;
		
		void postEvent(const VirtualEvent &event);
		void postEvents(const std::vector<VirtualEvent> &events);
		void processEvents();
		
		
	private:
		std::vector<VirtualEvent> m_events;
};


#endif

