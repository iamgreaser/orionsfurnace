vim: set sts=2 sw=2 et :

# Milestones

## Alpha 1

A few things before I inflict this upon people:

* [x] Implement the proper join sequence
* [ ] Implement disconnection
* [ ] Make the server open a TCP port
* [ ] Make the client use said TCP port
* [ ] Allow launching as a client or a nondedicated server
* [x] Put a donk^H^H^H^H licence on it

I think that's it for now. **EDIT: It wasn't it. This has been edited since putting it together. --GM**

----------------------------------------------------------------------------

# Misc

## Cleanups

* [ ] Get rid of the `Player::set_game()` kludge
* [ ] Use `GameFrame` for announcing new players?

## Features we'll need

* [ ] Implement a world
* [ ] Implement a camera so we're not stuck with 15x15
* [ ] Make headless server builds a thing
* [ ] Implement nicknames

## Ponies

* [ ] Add support for different underlying protocols
  * [ ] Add support for UDP
    * We cannot use ENet for this, IPv6 is a requirement these days.
  * [ ] Add support for SCTP
* [ ] Serious authentication

----------------------------------------------------------------------------

# Completed milestones

