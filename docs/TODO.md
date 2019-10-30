vim: set sts=2 sw=2 et :

# Milestones

## Alpha 3

* [x] Implement a camera so we're not stuck with 15x15
* [x] Implement a world
* [x] Use `GameFrame` for announcing new players
* [x] Windows-specific networking support

----------------------------------------------------------------------------

# Misc

## Cleanups

## Bugfixes

* [ ] Handle saves with mismatched player counts better
* [ ] Don't send player adds on top of game snapshots
  * There's a workaround for this, but I'd like to see it fixed properly.

## Features we'll need

* [ ] Handle non-contiguous player IDs
* [ ] Make it possible to remove players
* [ ] Make it possible for a client to save
* [ ] Make headless server builds a thing
* [ ] Implement nicknames
* [ ] Implement chat

## Ponies

* [ ] Add support for different underlying protocols
  * [ ] Add support for UDP
    * We cannot use ENet for this, IPv6 is a requirement these days.
  * [ ] Add support for SCTP
    * I think this would really be "do it without lagging like mad". It kinda works as-is, but it lags, so I'm probably doing something wrong. --GM
* [ ] Serious authentication

----------------------------------------------------------------------------

# Completed milestones

## Alpha 2

* [x] Report disconnects properly
* [x] Clean up the pointer misusage in favour of `shared_ptr`
  * There's probably more that can use it, or maybe we could use it a little less, but the remaining pointers are pretty much all backlinks.
* [x] Get rid of the `Player::set_game()` kludge
* [x] Either fix up the local pipes API, or throw it out
* [x] Make stuff like `TCP_NODELAY` happen

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

