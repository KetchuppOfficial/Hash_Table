# HT_Search of Version_0

```asm
00000000000025ac <HT_Search>:
    25ac:	f3 0f 1e fa          	endbr64 
    25b0:	41 54                	push   r12
    25b2:	55                   	push   rbp
    25b3:	53                   	push   rbx
    25b4:	48 89 fb             	mov    rbx,rdi
    25b7:	48 89 f5             	mov    rbp,rsi
    25ba:	48 89 f7             	mov    rdi,rsi
    25bd:	e8 f4 fd ff ff       	call   23b6 <crc_32>
    25c2:	ba 00 00 00 00       	mov    edx,0x0
    25c7:	f7 73 08             	div    DWORD PTR [rbx+0x8]
    25ca:	48 8b 03             	mov    rax,QWORD PTR [rbx]
    25cd:	48 8b 04 d0          	mov    rax,QWORD PTR [rax+rdx*8]
    25d1:	48 85 c0             	test   rax,rax
    25d4:	74 6c                	je     2642 <HT_Search+0x96>
    25d6:	48 8b 58 10          	mov    rbx,QWORD PTR [rax+0x10]
    25da:	48 85 db             	test   rbx,rbx
    25dd:	74 58                	je     2637 <HT_Search+0x8b>
    25df:	41 bc 00 00 00 00    	mov    r12d,0x0
    25e5:	eb 03                	jmp    25ea <HT_Search+0x3e>
    25e7:	48 89 d3             	mov    rbx,rdx
    25ea:	48 8b 48 08          	mov    rcx,QWORD PTR [rax+0x8]
    25ee:	48 8d 51 01          	lea    rdx,[rcx+0x1]
    25f2:	48 89 ee             	mov    rsi,rbp
    25f5:	48 8b 38             	mov    rdi,QWORD PTR [rax]
    25f8:	e8 93 fb ff ff       	call   2190 <memcmp@plt>
    25fd:	85 c0                	test   eax,eax
    25ff:	74 2e                	je     262f <HT_Search+0x83>
    2601:	48 8b 53 10          	mov    rdx,QWORD PTR [rbx+0x10]
    2605:	41 83 c4 01          	add    r12d,0x1
    2609:	48 89 d8             	mov    rax,rbx
    260c:	48 85 d2             	test   rdx,rdx
    260f:	75 d6                	jne    25e7 <HT_Search+0x3b>
    2611:	48 8b 43 08          	mov    rax,QWORD PTR [rbx+0x8]
    2615:	48 8d 50 01          	lea    rdx,[rax+0x1]
    2619:	48 89 ee             	mov    rsi,rbp
    261c:	48 8b 3b             	mov    rdi,QWORD PTR [rbx]
    261f:	e8 6c fb ff ff       	call   2190 <memcmp@plt>
    2624:	85 c0                	test   eax,eax
    2626:	b8 ff ff ff ff       	mov    eax,0xffffffff
    262b:	44 0f 45 e0          	cmovne r12d,eax
    262f:	44 89 e0             	mov    eax,r12d
    2632:	5b                   	pop    rbx
    2633:	5d                   	pop    rbp
    2634:	41 5c                	pop    r12
    2636:	c3                   	ret    
    2637:	48 89 c3             	mov    rbx,rax
    263a:	41 bc 00 00 00 00    	mov    r12d,0x0
    2640:	eb cf                	jmp    2611 <HT_Search+0x65>
    2642:	41 bc ff ff ff ff    	mov    r12d,0xffffffff
    2648:	eb e5                	jmp    262f <HT_Search+0x83>
```

# HT_Search of Version_1

```asm
0000000000001553 <HT_Search>:
    1553:	f3 0f 1e fa          	endbr64 
    1557:	55                   	push   rbp
    1558:	48 89 e5             	mov    rbp,rsp
    155b:	41 54                	push   r12
    155d:	53                   	push   rbx
    155e:	48 83 e4 e0          	and    rsp,0xffffffffffffffe0
    1562:	49 89 fc             	mov    r12,rdi
    1565:	48 89 f3             	mov    rbx,rsi
    1568:	48 89 f7             	mov    rdi,rsi
    156b:	e8 06 fe ff ff       	call   1376 <crc_32>
    1570:	ba 00 00 00 00       	mov    edx,0x0
    1575:	41 f7 74 24 08       	div    DWORD PTR [r12+0x8]
    157a:	49 8b 04 24          	mov    rax,QWORD PTR [r12]
    157e:	48 8b 04 d0          	mov    rax,QWORD PTR [rax+rdx*8]
    1582:	48 85 c0             	test   rax,rax
    1585:	0f 84 89 00 00 00    	je     1614 <HT_Search+0xc1>
    158b:	48 8b 50 08          	mov    rdx,QWORD PTR [rax+0x8]
    158f:	48 85 d2             	test   rdx,rdx
    1592:	74 76                	je     160a <HT_Search+0xb7>
    1594:	c5 fa 6f 1b          	vmovdqu xmm3,XMMWORD PTR [rbx]
    1598:	c4 e3 65 38 4b 10 01 	vinserti128 ymm1,ymm3,XMMWORD PTR [rbx+0x10],0x1
    159f:	b9 00 00 00 00       	mov    ecx,0x0
    15a4:	eb 03                	jmp    15a9 <HT_Search+0x56>
    15a6:	48 89 f2             	mov    rdx,rsi
    15a9:	48 8b 00             	mov    rax,QWORD PTR [rax]
    15ac:	c5 fa 6f 10          	vmovdqu xmm2,XMMWORD PTR [rax]
    15b0:	c4 e3 6d 38 40 10 01 	vinserti128 ymm0,ymm2,XMMWORD PTR [rax+0x10],0x1
    15b7:	c5 fd 74 c1          	vpcmpeqb ymm0,ymm0,ymm1
    15bb:	c5 fd d7 c0          	vpmovmskb eax,ymm0
    15bf:	83 f8 ff             	cmp    eax,0xffffffff
    15c2:	74 3b                	je     15ff <HT_Search+0xac>
    15c4:	48 8b 72 08          	mov    rsi,QWORD PTR [rdx+0x8]
    15c8:	83 c1 01             	add    ecx,0x1
    15cb:	48 89 d0             	mov    rax,rdx
    15ce:	48 85 f6             	test   rsi,rsi
    15d1:	75 d3                	jne    15a6 <HT_Search+0x53>
    15d3:	48 8b 02             	mov    rax,QWORD PTR [rdx]
    15d6:	c5 fa 6f 20          	vmovdqu xmm4,XMMWORD PTR [rax]
    15da:	c4 e3 5d 38 40 10 01 	vinserti128 ymm0,ymm4,XMMWORD PTR [rax+0x10],0x1
    15e1:	c5 fa 6f 2b          	vmovdqu xmm5,XMMWORD PTR [rbx]
    15e5:	c4 e3 55 38 4b 10 01 	vinserti128 ymm1,ymm5,XMMWORD PTR [rbx+0x10],0x1
    15ec:	c5 fd 74 c1          	vpcmpeqb ymm0,ymm0,ymm1
    15f0:	c5 fd d7 c0          	vpmovmskb eax,ymm0
    15f4:	83 f8 ff             	cmp    eax,0xffffffff
    15f7:	b8 ff ff ff ff       	mov    eax,0xffffffff
    15fc:	0f 45 c8             	cmovne ecx,eax
    15ff:	89 c8                	mov    eax,ecx
    1601:	48 8d 65 f0          	lea    rsp,[rbp-0x10]
    1605:	5b                   	pop    rbx
    1606:	41 5c                	pop    r12
    1608:	5d                   	pop    rbp
    1609:	c3                   	ret    
    160a:	48 89 c2             	mov    rdx,rax
    160d:	b9 00 00 00 00       	mov    ecx,0x0
    1612:	eb bf                	jmp    15d3 <HT_Search+0x80>
    1614:	b9 ff ff ff ff       	mov    ecx,0xffffffff
    1619:	eb e4                	jmp    15ff <HT_Search+0xac>
```