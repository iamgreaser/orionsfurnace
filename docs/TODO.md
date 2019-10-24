vim: set sts=2 sw=2 et :

# Milestones

## Alpha 1

A few things before I inflict this upon people:

* [ ] Implement the proper join sequence
* [ ] Make the server open a TCP port
* [ ] Make the client use said TCP port
* [ ] Allow launching as a client or a nondedicated server
* [ ] Put a donk^H^H^H^H licence on it

I think that's it for now.

----------------------------------------------------------------------------

# Misc

## Cleanups

* [ ] Get rid of the `Player::set_game()` kludge
* [ ] Use `GameFrame` for announcing new players?

## Features we'll need

* [ ] Implement a world
* [ ] Make headless server builds a thing

## Ponies

* [ ] Add support for different underlying protocols
  * [ ] Add support for UDP
    * We cannot use ENet for this, IPv6 is a requirement these days.
  * [ ] Add support for SCTP

----------------------------------------------------------------------------

# Completed milestones

