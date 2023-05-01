> Programming HW 2: C 언어로 chatting program 구현하기. (team HW)
이번 program은 io-multiplexing 기법을 활용하여 chatting program을 구현하는 것입니다.
> 

---

### 조건 in korean

1. 실습에 사용하는 컴퓨터는 203.252.112.25/26 을 사용하면 됩니다.
2. Socket program은 TCP를 사용하면 됩니다.
3. Chatting에 참여하고자 하는 사람은 server에 접속한 다음, 자신의 nick-name을 server에 등
록한다. 그러면 server는 현재 chatting에 참여하고 있는 사람들에게 새로운 사람 (nick-name)
의 chatting 참여를 알려주어야 한다. 그리고 새로 참여한 사람에게 현재 참여하고 있는 사람
들의 모든 nick-name을 알려주어야 한다.
4. Chatting에 참여하고 있는 사용자가 빠져나가면, server는 이를 현재 참여하고 있는 나머지
사람들에게 알려주어야 한다.
5. 어떤 사람이 메시지를 입력하면 서버를 이 메시지를 받은 다음, 그 메시지 앞에 보낸 사람의
nick-name을 추가한 다음, 현재 참여하고 있는 나머지 사람들에게 메시지를 전달하여야 한다.
6. 언제든지 chatting에 참여할 수 있고, 또한 빠져 나갈 수 있어야 한다.
7. Server program에서는 자신의 port number를 arguments로 받아서 실행되도록 program을 작
성하여야 한다.
8. Client program은 server의 IP address, server의 port number, 자신의 nick-name을 arguments
로 받아서 실행되도록 program을 작성하여야 한다.
9. 작성한 program은 ~/hw2/server, ~/hw2/client 에 위치하도록 하면 된다. (203.252.112.25와
26 컴퓨터에 어느 한 쪽에 server, 다른 쪽에 client 위치하도록 하면 됨.)
10. 결과물로는 설계한 program의 동작을 설명하고, 여러가지 시나리오에 따른 동작의 결과에
대해서 기술하면 됩니다.