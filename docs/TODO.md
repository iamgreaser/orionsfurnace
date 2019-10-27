vim: set sts=2 sw=2 et :

# Milestones

## Alpha 1

A few things before I inflict this upon people:

* [x] Implement the proper join sequence
* [x] Implement disconnection
* [x] Implement a send/recv over pipe interface and use it - needed for the next 2 or 3:
* [x] Make the server open a TCP port
* [x] Make the client use said TCP port
* [x] Allow launching as a client or a nondedicated server
* [x] Put a donk^H^H^H^H licence on it

I think that's it for now. **EDIT: It wasn't it. This has been edited since putting it together. --GM**

----------------------------------------------------------------------------

# Misc

## Cleanups

* [x] Report disconnects properly
* [ ] Clean up the pointer misusage in favour of `shared_ptr`
* [x] Get rid of the `Player::set_game()` kludge
* [ ] Either fix up the local pipes API, or throw it out
* [ ] Use `GameFrame` for announcing new players?

## Features we'll need

* [ ] Make it possible to remove players
* [ ] Make it possible for a client to save
* [x] Make stuff like `TCP_NODELAY` happen
* [ ] Handle saves with mismatched player counts better
* [ ] Windows-specific networking support
* [ ] Implement a world
* [ ] Implement a camera so we're not stuck with 15x15
* [ ] Make headless server builds a thing
* [ ] Implement nicknames

## Ponies

* [ ] Add support for different underlying protocols
  * [ ] Add support for UDP
    * We cannot use ENet for this, IPv6 is a requirement these days.
  * [ ] Add support for SCTP
    * I think this would really be "do it without lagging like mad". It kinda works as-is, but it lags, so I'm probably doing something wrong. --GM
* [ ] Serious authentication

----------------------------------------------------------------------------

# Completed milestones

