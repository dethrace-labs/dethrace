#ifndef _OPPONENT_H_
#define _OPPONENT_H_

#include "br_types.h"
#include "dr_types.h"

// Offset: 0
// Size: 137
// EAX: pThe_actor
// EDX: pThe_vector
void PointActorAlongThisBloodyVector(br_actor* pThe_actor, br_vector3* pThe_vector);

// Offset: 140
// Size: 301
// EAX: pOpponent_spec
// EDX: pCommand
void ProcessCurrentObjective(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand);

// Offset: 444
// Size: 190
// EAX: pHow_many_then
tS16 ReallocExtraPathNodes(int pHow_many_then);

// Offset: 636
// Size: 188
// EAX: pHow_many_then
tS16 ReallocExtraPathSections(int pHow_many_then);

// Offset: 824
// Size: 186
// EAX: pFrom
// EDX: pTo
int PointVisibleFromHere(br_vector3* pFrom, br_vector3* pTo);

// Offset: 1012
// Size: 276
// EAX: pActor_coords
// EDX: pDistance
tS16 FindNearestPathNode(br_vector3* pActor_coords, br_scalar* pDistance);

// Offset: 1288
// Size: 66
// EAX: pActor_coords
// EDX: pPath_direction
// EBX: pIntersect
// ECX: pDistance
tS16 FindNearestPathSection(br_vector3* pActor_coords, br_vector3* pPath_direction, br_vector3* pIntersect, br_scalar* pDistance);

// Offset: 1356
// Size: 1187
// EAX: pPursuee
// EDX: pActor_coords
// EBX: pPath_direction
// ECX: pIntersect
tS16 FindNearestGeneralSection(tCar_spec* pPursuee, br_vector3* pActor_coords, br_vector3* pPath_direction, br_vector3* pIntersect, br_scalar* pDistance);

// Offset: 2544
// Size: 171
// EAX: pCar_spec
void DeadStopCar(tCar_spec* pCar_spec);

// Offset: 2716
// Size: 69
// EAX: pOpponent_spec
void TurnOpponentPhysicsOn(tOpponent_spec* pOpponent_spec);

// Offset: 2788
// Size: 80
// EAX: pOpponent_spec
void TurnOpponentPhysicsOff(tOpponent_spec* pOpponent_spec);

// Offset: 2868
// Size: 247
void NewObjective(tOpponent_spec* pOpponent_spec, tOpponent_objective_type pObjective_type, ...);

// Offset: 3116
// Size: 929
// EAX: pOpponent_spec
void CalcRaceRoute(tOpponent_spec* pOpponent_spec);

// Offset: 4048
// Size: 790
// EAX: pOpponent_spec
// EDX: pSections_to_add
void TopUpRandomRoute(tOpponent_spec* pOpponent_spec, int pSections_to_add);

// Offset: 4840
// Size: 942
// EAX: pTemp_store
// EDX: pPerm_store
// EBX: pNum_of_perm_store_sections
// ECX: pTarget_section
int SearchForSection(tRoute_section* pTemp_store, tRoute_section* pPerm_store, int* pNum_of_perm_store_sections, tS16 pTarget_section, int pDepth, br_scalar pDistance_so_far, tOpponent_spec* pOpponent_spec);

// Offset: 5784
// Size: 796
// EAX: pOpponent_spec
// EDX: pPlayer
void CalcGetNearPlayerRoute(tOpponent_spec* pOpponent_spec, tCar_spec* pPlayer);

// Offset: 6580
// Size: 575
// EAX: pOpponent_spec
void CalcReturnToStartPointRoute(tOpponent_spec* pOpponent_spec);

// Offset: 7156
// Size: 47
// EAX: pOpponent_spec
void ClearOpponentsProjectedRoute(tOpponent_spec* pOpponent_spec);

// Offset: 7204
// Size: 114
// EAX: pOpponent_spec
// EDX: pSection_no
// EBX: pDirection
int AddToOpponentsProjectedRoute(tOpponent_spec* pOpponent_spec, tS16 pSection_no, int pDirection);

// Offset: 7320
// Size: 177
// EAX: pOpponent_spec
// EDX: pPlaces
int ShiftOpponentsProjectedRoute(tOpponent_spec* pOpponent_spec, int pPlaces);

// Offset: 7500
// Size: 134
// EAX: pOpponent_spec
// EDX: pMilliseconds
void StunTheBugger(tOpponent_spec* pOpponent_spec, int pMilliseconds);

// Offset: 7636
// Size: 47
// EAX: pOpponent_spec
void UnStunTheBugger(tOpponent_spec* pOpponent_spec);

// Offset: 7684
// Size: 436
// EAX: pOpponent_spec
// EDX: pCommand
void ProcessCompleteRace(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand);

// Offset: 8120
// Size: 337
// EAX: pPursuee
void StartRecordingTrail(tCar_spec* pPursuee);

// Offset: 8460
// Size: 1039
// EAX: pPursuee
void RecordNextTrailNode(tCar_spec* pPursuee);

// Offset: 9500
// Size: 78
// EAX: pOpponent_spec
// EDX: pPursuee
// EBX: pSection_v
// ECX: pIntersect
tS16 FindNearestTrailSection(tOpponent_spec* pOpponent_spec, tCar_spec* pPursuee, br_vector3* pSection_v, br_vector3* pIntersect, br_scalar* pDistance);

// Offset: 9580
// Size: 111
// EAX: pOpponent_spec
// EDX: pSection
tS16 CalcNextTrailSection(tOpponent_spec* pOpponent_spec, int pSection);

// Offset: 9692
// Size: 2515
// EAX: pOpponent_spec
// EDX: pCommand
void ProcessPursueAndTwat(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand);

// Offset: 12208
// Size: 638
// EAX: pOpponent_spec
// EDX: pCommand
void ProcessRunAway(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand);

// Offset: 12848
// Size: 126
// EAX: pOpponent_spec
// EDX: pCommand
void ProcessWaitForSomeHaplessSod(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand);

// Offset: 12976
// Size: 629
// EAX: pOpponent_spec
// EDX: pCommand
void ProcessReturnToStart(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand);

// Offset: 13608
// Size: 921
// EAX: pOpponent_spec
// EDX: pCommand
void ProcessLevitate(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand);

// Offset: 14532
// Size: 543
// EAX: pOpponent_spec
// EDX: pCommand
void ProcessGetNearPlayer(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand);

// Offset: 15076
// Size: 230
// EAX: pOpponent_spec
// EDX: pCommand
void ProcessFrozen(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand);

// Offset: 15308
// Size: 355
// EAX: pOpponent_spec
int HeadOnWithPlayerPossible(tOpponent_spec* pOpponent_spec);

// Offset: 15664
// Size: 88
// EAX: pOpponent_spec
// EDX: pPursuee
int AlreadyPursuingCar(tOpponent_spec* pOpponent_spec, tCar_spec* pPursuee);

// Offset: 15752
// Size: 97
// EAX: pOpponent_spec
int LastTwatteeAPlayer(tOpponent_spec* pOpponent_spec);

// Offset: 15852
// Size: 97
// EAX: pOpponent_spec
int LastTwatterAPlayer(tOpponent_spec* pOpponent_spec);

// Offset: 15952
// Size: 130
// EAX: pOpponent_spec
void ObjectiveComplete(tOpponent_spec* pOpponent_spec);

// Offset: 16084
// Size: 542
// EAX: pOpponent_spec
void TeleportOpponentToNearestSafeLocation(tOpponent_spec* pOpponent_spec);

// Offset: 16628
// Size: 4057
// EAX: pOpponent_spec
// EDX: pMust_choose_one
void ChooseNewObjective(tOpponent_spec* pOpponent_spec, int pMust_choose_one);

// Offset: 20688
// Size: 365
// EAX: pOpponent_spec
void ProcessThisOpponent(tOpponent_spec* pOpponent_spec);

// Offset: 21056
// Size: 256
// EAX: pPoint
int IsNetCarActive(br_vector3* pPoint);

// Offset: 21312
// Size: 692
void RebuildActiveCarList();

// Offset: 22004
// Size: 58
void ForceRebuildActiveCarList();

// Offset: 22064
// Size: 138
// EAX: pOpponent_spec
void StartToCheat(tOpponent_spec* pOpponent_spec);

// Offset: 22204
// Size: 138
// EAX: pOpponent_spec
void OiStopCheating(tOpponent_spec* pOpponent_spec);

// Offset: 22344
// Size: 326
// EAX: pOpponent_spec
int TeleportCopToStart(tOpponent_spec* pOpponent_spec);

// Offset: 22672
// Size: 139
// EAX: pOpponent_spec
void CalcDistanceFromHome(tOpponent_spec* pOpponent_spec);

// Offset: 22812
// Size: 767
// EAX: pOpponent_spec
// EDX: pMassage_count
int MassageOpponentPosition(tOpponent_spec* pOpponent_spec, int pMassage_count);

// Offset: 23580
// Size: 821
// EAX: pOpponent_spec
int RematerialiseOpponentOnThisSection(tOpponent_spec* pOpponent_spec, br_scalar pSpeed, tS16 pSection_no);

// Offset: 24404
// Size: 482
// EAX: pOpponent_spec
int RematerialiseOpponentOnNearestSection(tOpponent_spec* pOpponent_spec, br_scalar pSpeed);

// Offset: 24888
// Size: 2365
// EAX: pOpponent_spec
int RematerialiseOpponent(tOpponent_spec* pOpponent_spec, br_scalar pSpeed);

// Offset: 27256
// Size: 434
// EAX: pOpponent_spec
void CalcPlayerConspicuousness(tOpponent_spec* pOpponent_spec);

// Offset: 27692
// Size: 192
// EAX: pOpponent_spec
void CalcOpponentConspicuousnessWithAViewToCheatingLikeFuck(tOpponent_spec* pOpponent_spec);

// Offset: 27884
// Size: 52
// EAX: pChallenger_index
// EDX: pAccepted
void ChallengeOccurred(int pChallenger_index, int pAccepted);

// Offset: 27936
// Size: 232
void LoadCopCars();

// Offset: 28168
// Size: 2460
// EAX: pF
void LoadInOppoPaths(FILE* pF);

// Offset: 30628
// Size: 153
void DisposeOpponentPaths();

// Offset: 30784
// Size: 950
// EAX: pFrame_period
void MungeOpponents(tU32 pFrame_period);

// Offset: 31736
// Size: 251
void SetInitialCopPositions();

// Offset: 31988
// Size: 2545
// EAX: pRace_info
void InitOpponents(tRace_info* pRace_info);

// Offset: 34536
// Size: 142
void DisposeOpponents();

// Offset: 34680
// Size: 311
// EAX: pWhat_the_countdown_was
void WakeUpOpponentsToTheFactThatTheStartHasBeenJumped(int pWhat_the_countdown_was);

// Offset: 34992
// Size: 91
// EAX: pCar_spec
void ReportMurderToPoliceDepartment(tCar_spec* pCar_spec);

// Offset: 35084
// Size: 178
// EAX: pCategory
int GetCarCount(tVehicle_type pCategory);

// Offset: 35264
// Size: 238
// EAX: pCategory
// EDX: pIndex
tCar_spec* GetCarSpec(tVehicle_type pCategory, int pIndex);

// Offset: 35504
// Size: 172
// EAX: pCategory
// EDX: pIndex
char* GetDriverName(tVehicle_type pCategory, int pIndex);

// Offset: 35676
// Size: 240
// EAX: pCar_spec
tOpponent_spec* GetOpponentSpecFromCarSpec(tCar_spec* pCar_spec);

// Offset: 35916
// Size: 113
// EAX: pIndex
tCar_spec* GetCarSpecFromGlobalOppoIndex(int pIndex);

// Offset: 36032
// Size: 95
// EAX: pOpponent_spec
// EDX: pSection_no
int GetOpponentsRealSection(tOpponent_spec* pOpponent_spec, int pSection_no);

// Offset: 36128
// Size: 106
// EAX: pOpponent_spec
int GetOpponentsFirstSection(tOpponent_spec* pOpponent_spec);

// Offset: 36236
// Size: 185
// EAX: pOpponent_spec
// EDX: pCurrent_section
int GetOpponentsNextSection(tOpponent_spec* pOpponent_spec, tS16 pCurrent_section);

// Offset: 36424
// Size: 239
// EAX: pOpponent_spec
// EDX: pSection
tS16 GetOpponentsSectionStartNode(tOpponent_spec* pOpponent_spec, tS16 pSection);

// Offset: 36664
// Size: 185
// EAX: pOpponent_spec
// EDX: pSection
tS16 GetOpponentsSectionFinishNode(tOpponent_spec* pOpponent_spec, tS16 pSection);

// Offset: 36852
// Size: 284
// EAX: pOpponent_spec
// EDX: pSection
br_vector3* GetOpponentsSectionStartNodePoint(tOpponent_spec* pOpponent_spec, tS16 pSection);

// Offset: 37136
// Size: 266
// EAX: pOpponent_spec
// EDX: pSection
br_vector3* GetOpponentsSectionFinishNodePoint(tOpponent_spec* pOpponent_spec, tS16 pSection);

// Offset: 37404
// Size: 165
// EAX: pOpponent_spec
// EDX: pSection
br_scalar GetOpponentsSectionWidth(tOpponent_spec* pOpponent_spec, tS16 pSection);

// Offset: 37572
// Size: 262
// EAX: pOpponent_spec
// EDX: pSection
// EBX: pTowards_finish
int GetOpponentsSectionMinSpeed(tOpponent_spec* pOpponent_spec, tS16 pSection, int pTowards_finish);

// Offset: 37836
// Size: 262
// EAX: pOpponent_spec
// EDX: pSection
// EBX: pTowards_finish
int GetOpponentsSectionMaxSpeed(tOpponent_spec* pOpponent_spec, tS16 pSection, int pTowards_finish);

// Offset: 38100
// Size: 59
// EAX: pOpponent_index
void InitOpponentPsyche(int pOpponent_index);

// Offset: 38160
// Size: 108
// EAX: pOpponent_spec
void ClearTwattageOccurrenceVariables(tOpponent_spec* pOpponent_spec);

// Offset: 38268
// Size: 86
// EAX: pA_car
// EDX: pAnother_car
void TwoCarsHitEachOther(tCar_spec* pA_car, tCar_spec* pAnother_car);

// Offset: 38356
// Size: 1166
// EAX: pTwatter
// EDX: pTwattee
void RecordOpponentTwattageOccurrence(tCar_spec* pTwatter, tCar_spec* pTwattee);

// Offset: 39524
// Size: 67
void ToggleOpponentTest();

// Offset: 39592
// Size: 342
void ToggleOpponentProcessing();

// Offset: 39936
// Size: 183
void ToggleMellowOpponents();

// Offset: 40120
// Size: 184
void RepairOpponentsSystems();

// Offset: 40564
// Size: 530
// EAX: pSection_to_delete
void DeleteSection(tS16 pSection_to_delete);

// Offset: 41096
// Size: 824
// EAX: pNode_to_delete
// EDX: pAnd_sections
void DeleteNode(tS16 pNode_to_delete, int pAnd_sections);

// Offset: 41920
// Size: 116
void DeleteOrphanNodes();

// Offset: 42036
// Size: 716
// EAX: pInserted_node
// EDX: pSection_no
// EBX: pWhere
void InsertThisNodeInThisSectionHere(tS16 pInserted_node, tS16 pSection_no, br_vector3* pWhere);

// Offset: 42752
// Size: 254
void TrackElasticateyPath();

// Offset: 43008
// Size: 1043
// EAX: pMax_not_min
// EDX: pAdjustment
void RecalcNearestPathSectionSpeed(int pMax_not_min, int pAdjustment);

// Offset: 44052
// Size: 320
void RecalcNearestPathSectionWidth(br_scalar pAdjustment);

// Offset: 44372
// Size: 325
// EAX: pNegative_x_vector
// EDX: pStart
// EBX: pFinish
void CalcNegativeXVector(br_vector3* pNegative_x_vector, br_vector3* pStart, br_vector3* pFinish, br_scalar pLength);

// Offset: 44700
// Size: 162
// EAX: pModel
// EDX: pVertex_num
void MakeVertexAndOffsetIt(br_model* pModel, int pVertex_num, br_scalar pX, br_scalar pY, br_scalar pZ, br_vector3* pOffset);

// Offset: 44864
// Size: 134
// EAX: pModel
// EDX: pFace_num
// EBX: pV0
// ECX: pV1
void MakeFaceAndTextureIt(br_model* pModel, int pFace_num, int pV0, int pV1, int pV2, br_material* pMaterial);

// Offset: 45000
// Size: 942
// EAX: pFirst_vertex
// EDX: pFirst_face
// EBX: pStart
// ECX: pFinish
void MakeSection(br_uint_16 pFirst_vertex, br_uint_16 pFirst_face, br_vector3* pStart, br_vector3* pFinish, br_scalar pWidth, br_material* pMaterial_centre_lt, br_material* pMaterial_centre_dk, br_material* pMaterial_edges_start_lt, br_material* pMaterial_edges_start_dk, br_material* pMaterial_edges_finish_lt, br_material* pMaterial_edges_finish_dk);

// Offset: 45944
// Size: 1235
// EAX: pFirst_vertex
// EDX: pFirst_face
// EBX: pPoint
// ECX: pMaterial_1
void MakeCube(br_uint_16 pFirst_vertex, br_uint_16 pFirst_face, br_vector3* pPoint, br_material* pMaterial_1, br_material* pMaterial_2, br_material* pMaterial_3);

// Offset: 47180
// Size: 88
// EAX: pFace_index_ptr
// EDX: pVertex_index_ptr
void CalcNumberOfFacesAndVerticesForOppoPathModel(br_uint_16* pFace_index_ptr, br_uint_16* pVertex_index_ptr);

// Offset: 47268
// Size: 433
// EAX: pModel
// EDX: pNum_faces
// EBX: pNum_vertices
void ReallocModelFacesAndVertices(br_model* pModel, int pNum_faces, int pNum_vertices);

// Offset: 47704
// Size: 127
// EAX: pColour_index
br_material* CreateSimpleMaterial(int pColour_index);

// Offset: 47832
// Size: 254
void AllocateMatsForOppoPathModel();

// Offset: 48088
// Size: 1093
void RebuildOppoPathModel();

// Offset: 49184
// Size: 1319
int ConsistencyCheck();

// Offset: 50504
// Size: 172
void ShowOppoPaths();

// Offset: 50676
// Size: 832
void WriteOutOppoPaths();

// Offset: 51508
// Size: 338
int NewNodeOKHere();

// Offset: 51848
// Size: 81
void ShowHideOppoPaths();

// Offset: 51932
// Size: 1148
void DropElasticateyNode();

// Offset: 53080
// Size: 1216
void InsertAndElasticate();

// Offset: 54296
// Size: 623
void InsertAndDontElasticate();

// Offset: 54920
// Size: 300
void DropDeadEndNode();

// Offset: 55220
// Size: 404
void DropNodeOnNodeAndStopElasticating();

// Offset: 55624
// Size: 53
void WidenOppoPathSection();

// Offset: 55680
// Size: 53
void NarrowOppoPathSection();

// Offset: 55736
// Size: 55
void IncreaseSectionMinSpeed();

// Offset: 55792
// Size: 55
void DecreaseSectionMinSpeed();

// Offset: 55848
// Size: 58
void IncreaseSectionMaxSpeed();

// Offset: 55908
// Size: 58
void DecreaseSectionMaxSpeed();

// Offset: 55968
// Size: 260
void PullOppoPoint();

// Offset: 56228
// Size: 301
void ShowNodeInfo();

// Offset: 56532
// Size: 364
void ShowSectionInfo1();

// Offset: 56896
// Size: 717
void ShowSectionInfo2();

// Offset: 57616
// Size: 226
void DeleteOppoPathSection();

// Offset: 57844
// Size: 225
void DeleteOppoPathNodeAndSections();

// Offset: 58072
// Size: 535
void DeleteOppoPathNodeAndJoin();

// Offset: 58608
// Size: 424
void ReverseSectionDirection();

// Offset: 59032
// Size: 325
void CycleSectionType();

// Offset: 59360
// Size: 321
void ToggleOneWayNess();

// Offset: 59684
// Size: 350
void CopStartPointInfo();

// Offset: 60036
// Size: 318
void DropCopStartPoint();

// Offset: 60356
// Size: 504
void DeleteCopStartPoint();

// Offset: 60860
// Size: 34
void CycleCopStartPointType();

#endif
