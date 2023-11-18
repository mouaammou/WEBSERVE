+------------------------+
|   Start the Server     |
|   Initialization       |
+------------+-----------+
             |
             V
  +--------------------------+
  |   Create Server Socket   |
  |   and Set it to Listen   |
  +------------+-------------+
               |
               V
  +--------------------------+
  |       Set Up Polling      |
  |    Array for File Descriptors |
  |    (including Server Socket)  |
  +-----------------------------+
                |
                V
  +---------------------------+
  |  Loop: Wait for Events on |
  |      Multiple Descriptors   |
  +--------------+------------+
                 |
                 V
  +----------------------------+
  |   Check for Events on the   |
  |        Server Socket (New   |
  |        Client Connection)   |
  +--------------+--------------+
                 |
                 | (New Client Connection)
                 V
  +-----------------------------+
  |    Accept New Client       |
  |    Connection and Add it to |
  |    the Polling Array         |
  +-----------------------------+
                |
                V
  +---------------------------+
  | Loop: Check for Events on |
  |     Connected Clients      |
  +--------------+------------+
                 |
                 | (Data from Client)
                 V
  +-----------------------------+
  |    Handle Data from Client   |
  |    (Read, Write, Process,    |
  |    etc.)                      |
  +-----------------------------+
                |
                V
  +-----------------------------+
  |     Close Client Connection  |
  |     if Needed (e.g., client   |
  |     disconnects)             |
  +-----------------------------+
                |
                V
  +---------------------------+
  | Loop: Check for Events on |
  |     Other Connected Clients |
  +--------------+------------+
                 |
                 V
  +-----------------------------+
  |    Repeat Until Server is    |
  |    Stopped or Other Exit     |
  |    Condition is Met          |
  +-----------------------------+
                |
                V
  +---------------------------+
  |        Close Server        |
  |        and Cleanup         |
  +------------+--------------+
               |
               V
         +-----------+
         |   End     |
         +-----------+
