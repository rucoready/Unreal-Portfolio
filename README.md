# Unreal-Portfolio

포트폴리오 목표로 제작된 멀티 RPG 데모 게임입니다.<br/>
<br/>
● 엔진 : Unreal Engine5 5.4 <br/>
● 규모 : 3인 <br/>
● 제작기간 : 4주 <br/>

# 사용한 개발 도구<br/>
● Microsoft Visual Studio Community<br/>
● Blender<br/>
● GithubDesktop<br/>

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
● ㅇ
● 제작방식 : BehaviorTree <br/>
| Phase1 Nodes | Phase2 Nodes |
|:--:|:--:|
| ![Phase1](https://github.com/user-attachments/assets/b2c2124e-21c3-4aa2-9dfe-5cccb43684e6)<br/><sub></sub> | ![Phase2](https://github.com/user-attachments/assets/cfef0d85-6d41-4863-98f6-33b1b51c47c4)<br/><sub></sub> |

| Pattern Nodes | All Nodes |
|:--:|:--:|
| ![Phase3](https://github.com/user-attachments/assets/661f0aaf-756f-4a8a-81f1-82f447ea6222)<br/><sub></sub> | ![Phase4](https://github.com/user-attachments/assets/088dfdb6-a959-4a48-9530-d5d0f2578e5d)<br/><sub></sub> |<br/>

● Service Node, Decorator Node를 사용한 여러가지 조건 체크<br/>
● Blackboard과 BehaviorTree의 상호작용<br/>
● 각각 Task에서 독립적인 패턴 구현<br/>

