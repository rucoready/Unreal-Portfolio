# Unreal-Portfolio

포트폴리오 목표로 제작된 멀티 RPG 데모 게임입니다.<br/>
<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 엔진 : Unreal Engine5 5.4 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 규모 : 3인 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 제작기간 : 4주 <br/>

# 사용한 개발 도구<br/>

&nbsp;&nbsp;&nbsp;&nbsp;● Microsoft Visual Studio Community<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Blender<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● GithubDesktop<br/>

# 데모 영상 
| SinglePlay | MultiPlay |
| --- | --- |
| <a href="https://youtu.be/PPo4rxRjsWc"><img src="https://img.youtube.com/vi/PPo4rxRjsWc/0.jpg" width="300"> </a> | <br/><a href="https://youtu.be/9zh2KllxqSw"><img src="https://img.youtube.com/vi/9zh2KllxqSw/0.jpg" width="300"></a> | <br/>

● SinglePlayer 영상 -> CombatSystem 설명 <br/>
● SinglePlayer 영상 -> 멀티플레이 및 크래프팅 설명<br/>

# Packging File
● Link : https://drive.google.com/file/d/1Kx00sBZXGBgPO2W_-GXQI02bvHIhqV7Y/view?usp=drive_link <br/>

# 기술 설명

## Boss
던전 마지막에 배치된 복잡한 AI<br/>

&nbsp;&nbsp;&nbsp;&nbsp;● BehaviorTree 동작방식 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● BossWidget 관리 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● BossBlackBoard 관리 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 네트워크 동기화 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Service Node, Decorator Node를 사용한 여러가지 조건 체크<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Blackboard과 BehaviorTree의 상호작용<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 각각 Task에서 독립적인 패턴 구현<br/>

| Phase1 Nodes | Phase2 Nodes |
|:--:|:--:|
| ![Phase1](https://github.com/user-attachments/assets/b2c2124e-21c3-4aa2-9dfe-5cccb43684e6)<br/><sub></sub> | ![Phase2](https://github.com/user-attachments/assets/cfef0d85-6d41-4863-98f6-33b1b51c47c4)<br/><sub></sub> |

| Pattern Nodes | All Nodes |
|:--:|:--:|
| ![Phase3](https://github.com/user-attachments/assets/661f0aaf-756f-4a8a-81f1-82f447ea6222)<br/><sub></sub> | ![Phase4](https://github.com/user-attachments/assets/088dfdb6-a959-4a48-9530-d5d0f2578e5d)<br/><sub></sub> |<br/>

## Enemy AI
맵에 배치된 기본적인 적AI<br/>

&nbsp;&nbsp;&nbsp;&nbsp;● BehaviorTree 동작방식 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● StateMachine 상반신 하반신 개별화 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 독자적 개인 시야각 적용 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 데미지 출력 HUD <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 여러가지 아이템 확률드랍 <br/>
| Grux | Dog |
|:--:|:--:|
| <div align="center"> ![왼쪽 위](https://github.com/user-attachments/assets/df94cdcc-cfee-4583-9248-461bde2f5472) </div> | <div align="center"> ![오른쪽 위](https://github.com/user-attachments/assets/bbd4e4be-a4dd-4b1b-ad49-3ab117255f86) </div> |
| <div align="center"> ![왼쪽 아래](https://github.com/user-attachments/assets/4eaa73d2-e43c-45a0-a112-fc89318e229a) </div> | <div align="center"> ![오른쪽 아래](https://github.com/user-attachments/assets/8a78faf6-cc62-4232-8241-b12da5d8bef6) </div> |


