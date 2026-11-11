#pragma once
#include "Observer.h"
#include "SinglePortModule.h"

/**
 * Waypoint message handling for meshtastic
 */
class WaypointModule : public SinglePortModule, public Observable<const UIFrameEvent *>
{
  public:
    /** Constructor
     * name is for debugging output
     */
    WaypointModule() : SinglePortModule("waypoint", meshtastic_PortNum_WAYPOINT_APP) {}
#if HAS_SCREEN
    bool shouldDraw();
    void onDeviceTimeChanged();
#endif
#if !MESHTASTIC_EXCLUDE_WAYPOINT
    /// Broadcast an expired copy of the waypoint so the mesh (and we) discard it.
    bool broadcastDelete(uint32_t waypointId);

    /// Broadcast a brand-new waypoint owned by this radio. expireEpoch of 0 means never expires.
    /// Our own packet loops back through handleReceived(), so the new waypoint lands in the store
    /// and on flash without a second save call.
    bool broadcastCreate(const char *name, double lat, double lon, uint32_t expireEpoch = 0);

    /// broadcastCreate() at our own last known position. False when there is no fix yet.
    bool createFromCurrentPosition(const char *name, uint32_t expireEpoch = 0);
#endif
  protected:
    /** Called to handle a particular incoming message

    @return ProcessMessage::STOP if you've guaranteed you've handled this message and no other handlers should be considered for
    it
    */

    virtual Observable<const UIFrameEvent *> *getUIFrameObservable() override { return this; }
#if HAS_SCREEN
    virtual bool wantUIFrame() override { return this->shouldDraw(); }
    virtual void drawFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y) override;
#endif
    virtual ProcessMessage handleReceived(const meshtastic_MeshPacket &mp) override;
};

extern WaypointModule *waypointModule;
