#ifndef _OPPONENT_H_
#define _OPPONENT_H_

#include "dr_types.h"

#define CAR_SPEC_IS_OPPONENT(CAR_SPEC) (VEHICLE_TYPE_FROM_ID((CAR_SPEC)->car_ID) == eVehicle_opponent)
#define CAR_SPEC_IS_ROZZER(CAR_SPEC) (VEHICLE_TYPE_FROM_ID((CAR_SPEC)->car_ID) == eVehicle_rozzer)

#define CAR_SPEC_GET_SPEED_FACTOR(CAR_SPEC) (CAR_SPEC_IS_ROZZER(CAR_SPEC) ? gCop_speed_factor : gOpponent_speed_factor)

extern br_actor* gOppo_path_actor;
extern br_model* gOppo_path_model;
extern br_material* gMat_dk_yel;
extern br_material* gMat_md_yel;
extern br_material* gMat_lt_yel;
extern br_material* gMat_dk_red;
extern br_material* gMat_lt_red;
extern br_material* gMat_dk_grn;
extern br_material* gMat_lt_grn;
extern br_material* gMat_dk_blu;
extern br_material* gMat_lt_blu;
extern br_material* gMat_dk_turq;
extern br_material* gMat_lt_turq;
extern br_material* gMat_dk_gry;
extern br_material* gMat_md_gry;
extern br_material* gMat_lt_gry;
extern int gMellow_opponents;
extern int gTest_toggle;
extern int gAlready_elasticating;
extern int gVertices_used_in_non_edit_paths;
extern int gFaces_used_in_non_edit_paths;
extern int gMats_allocated;
extern int gOppo_paths_shown;
extern int gMade_path_filename;
extern int gBIG_APC_index;
extern char* gPath_section_type_names[3];
extern int gMin_bangness;
extern int gMax_bangness;
extern tU32 gNext_elastication;
extern tU32 gNext_write_during_elastication;
extern char* gCop_name;
extern char* gDrone_name;
extern int gChallenger_index__opponent; // suffix added to avoid duplicate symbol
extern int gSFS_count;
extern int gSFS_total_cycles;
extern int gSFS_max_cycles;
extern float gOpponent_nastyness_frigger;
extern char gOppo_path_filename[256];
extern br_scalar gIn_view_distance;
extern tU8* gBit_per_node;
extern int gGrudge_reduction_per_period;
extern int gSFS_cycles_this_time;
extern br_scalar gMinimum_yness_before_knackerisation;
extern int gWanky_arse_tit_fuck;
extern br_scalar gHead_on_cos_value;
extern tU32 gNext_grudge_reduction;
extern br_scalar gCop_pursuit_speed_percentage_multiplier;
extern br_scalar gDefinite_cop_pursuit_speed;
extern int gAcknowledged_start;
extern int gStart_jumped;
extern int gNum_of_opponents_getting_near;
extern int gNumber_of_cops_before_faffage;
extern int gFirst_frame;
extern tU32 gAcme_frame_count;
extern br_scalar gDefinite_no_cop_pursuit_speed;
extern int gNum_of_opponents_completing_race;
extern int gNum_of_opponents_pursuing;
extern int gActive_car_list_rebuild_required;
extern br_scalar gFrame_period_for_this_munging_in_secs;
extern int gBig_bang;
extern int gProcessing_opponents;
extern tU32 gFrame_period_for_this_munging;
extern tU32 gTime_stamp_for_this_munging;
extern tS16 gMobile_section;

void PointActorAlongThisBloodyVector(br_actor* pThe_actor, br_vector3* pThe_vector);

void ProcessCurrentObjective(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand);

tS16 ReallocExtraPathNodes(int pHow_many_then);

tS16 ReallocExtraPathSections(int pHow_many_then);

int PointVisibleFromHere(br_vector3* pFrom, br_vector3* pTo);

tS16 FindNearestPathNode(br_vector3* pActor_coords, br_scalar* pDistance);

tS16 FindNearestPathSection(br_vector3* pActor_coords, br_vector3* pPath_direction, br_vector3* pIntersect, br_scalar* pDistance);

tS16 FindNearestGeneralSection(tCar_spec* pPursuee, br_vector3* pActor_coords, br_vector3* pPath_direction, br_vector3* pIntersect, br_scalar* pDistance);

void DeadStopCar(tCar_spec* pCar_spec);

void TurnOpponentPhysicsOn(tOpponent_spec* pOpponent_spec);

void TurnOpponentPhysicsOff(tOpponent_spec* pOpponent_spec);

void NewObjective(tOpponent_spec* pOpponent_spec, tOpponent_objective_type pObjective_type, ...);

void CalcRaceRoute(tOpponent_spec* pOpponent_spec);

void TopUpRandomRoute(tOpponent_spec* pOpponent_spec, int pSections_to_add);

int SearchForSection(tRoute_section* pTemp_store, tRoute_section* pPerm_store, int* pNum_of_perm_store_sections, tS16 pTarget_section, int pDepth, br_scalar pDistance_so_far, tOpponent_spec* pOpponent_spec);

void CalcGetNearPlayerRoute(tOpponent_spec* pOpponent_spec, tCar_spec* pPlayer);

void CalcReturnToStartPointRoute(tOpponent_spec* pOpponent_spec);

void ClearOpponentsProjectedRoute(tOpponent_spec* pOpponent_spec);

int AddToOpponentsProjectedRoute(tOpponent_spec* pOpponent_spec, tS16 pSection_no, int pDirection);

int ShiftOpponentsProjectedRoute(tOpponent_spec* pOpponent_spec, int pPlaces);

void StunTheBugger(tOpponent_spec* pOpponent_spec, int pMilliseconds);

void UnStunTheBugger(tOpponent_spec* pOpponent_spec);

void ProcessCompleteRace(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand);

void StartRecordingTrail(tCar_spec* pPursuee);

void RecordNextTrailNode(tCar_spec* pPursuee);

tS16 FindNearestTrailSection(tOpponent_spec* pOpponent_spec, tCar_spec* pPursuee, br_vector3* pSection_v, br_vector3* pIntersect, br_scalar* pDistance);

tS16 CalcNextTrailSection(tOpponent_spec* pOpponent_spec, int pSection);

void ProcessPursueAndTwat(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand);

void ProcessRunAway(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand);

void ProcessWaitForSomeHaplessSod(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand);

void ProcessReturnToStart(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand);

void ProcessLevitate(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand);

void ProcessGetNearPlayer(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand);

void ProcessFrozen(tOpponent_spec* pOpponent_spec, tProcess_objective_command pCommand);

int HeadOnWithPlayerPossible(tOpponent_spec* pOpponent_spec);

int AlreadyPursuingCar(tOpponent_spec* pOpponent_spec, tCar_spec* pPursuee);

int LastTwatteeAPlayer(tOpponent_spec* pOpponent_spec);

int LastTwatterAPlayer(tOpponent_spec* pOpponent_spec);

void ObjectiveComplete(tOpponent_spec* pOpponent_spec);

void TeleportOpponentToNearestSafeLocation(tOpponent_spec* pOpponent_spec);

void ChooseNewObjective(tOpponent_spec* pOpponent_spec, int pMust_choose_one);

void ProcessThisOpponent(tOpponent_spec* pOpponent_spec);

int IsNetCarActive(br_vector3* pPoint);

void RebuildActiveCarList(void);

void ForceRebuildActiveCarList(void);

void StartToCheat(tOpponent_spec* pOpponent_spec);

void OiStopCheating(tOpponent_spec* pOpponent_spec);

int TeleportCopToStart(tOpponent_spec* pOpponent_spec);

void CalcDistanceFromHome(tOpponent_spec* pOpponent_spec);

int MassageOpponentPosition(tOpponent_spec* pOpponent_spec, int pMassage_count);

int RematerialiseOpponentOnThisSection(tOpponent_spec* pOpponent_spec, br_scalar pSpeed, tS16 pSection_no);

int RematerialiseOpponentOnNearestSection(tOpponent_spec* pOpponent_spec, br_scalar pSpeed);

int RematerialiseOpponent(tOpponent_spec* pOpponent_spec, br_scalar pSpeed);

void CalcPlayerConspicuousness(tOpponent_spec* pOpponent_spec);

void CalcOpponentConspicuousnessWithAViewToCheatingLikeFuck(tOpponent_spec* pOpponent_spec);

void ChallengeOccurred(int pChallenger_index, int pAccepted);

void LoadCopCars(void);

void LoadInOppoPaths(FILE* pF);

void DisposeOpponentPaths(void);

void MungeOpponents(tU32 pFrame_period);

void SetInitialCopPositions(void);

void InitOpponents(tRace_info* pRace_info);

void DisposeOpponents(void);

void WakeUpOpponentsToTheFactThatTheStartHasBeenJumped(int pWhat_the_countdown_was);

void ReportMurderToPoliceDepartment(tCar_spec* pCar_spec);

int GetCarCount(tVehicle_type pCategory);

tCar_spec* GetCarSpec(tVehicle_type pCategory, int pIndex);

char* GetDriverName(tVehicle_type pCategory, int pIndex);

tOpponent_spec* GetOpponentSpecFromCarSpec(tCar_spec* pCar_spec);

tCar_spec* GetCarSpecFromGlobalOppoIndex(int pIndex);

int GetOpponentsRealSection(tOpponent_spec* pOpponent_spec, int pSection_no);

int GetOpponentsFirstSection(tOpponent_spec* pOpponent_spec);

int GetOpponentsNextSection(tOpponent_spec* pOpponent_spec, tS16 pCurrent_section);

tS16 GetOpponentsSectionStartNode(tOpponent_spec* pOpponent_spec, tS16 pSection);

tS16 GetOpponentsSectionFinishNode(tOpponent_spec* pOpponent_spec, tS16 pSection);

br_vector3* GetOpponentsSectionStartNodePoint(tOpponent_spec* pOpponent_spec, tS16 pSection);

br_vector3* GetOpponentsSectionFinishNodePoint(tOpponent_spec* pOpponent_spec, tS16 pSection);

br_scalar GetOpponentsSectionWidth(tOpponent_spec* pOpponent_spec, tS16 pSection);

int GetOpponentsSectionMinSpeed(tOpponent_spec* pOpponent_spec, tS16 pSection, int pTowards_finish);

int GetOpponentsSectionMaxSpeed(tOpponent_spec* pOpponent_spec, tS16 pSection, int pTowards_finish);

void InitOpponentPsyche(int pOpponent_index);

void ClearTwattageOccurrenceVariables(tOpponent_spec* pOpponent_spec);

void TwoCarsHitEachOther(tCar_spec* pA_car, tCar_spec* pAnother_car);

void RecordOpponentTwattageOccurrence(tCar_spec* pTwatter, tCar_spec* pTwattee);

void ToggleOpponentTest(void);

void ToggleOpponentProcessing(void);

void ToggleMellowOpponents(void);

void RepairOpponentsSystems(void);

void CopyVertex__opponent(br_vertex* pDest_vertex, br_vertex* pSrc_vertex);

void CopyFace__opponent(br_face* pDest_face, br_face* pSrc_face);

void DeleteSection(tS16 pSection_to_delete);

void DeleteNode(tS16 pNode_to_delete, int pAnd_sections);

void DeleteOrphanNodes(void);

void InsertThisNodeInThisSectionHere(tS16 pInserted_node, tS16 pSection_no, br_vector3* pWhere);

void TrackElasticateyPath(void);

void RecalcNearestPathSectionSpeed(int pMax_not_min, int pAdjustment);

void RecalcNearestPathSectionWidth(br_scalar pAdjustment);

void CalcNegativeXVector(br_vector3* pNegative_x_vector, br_vector3* pStart, br_vector3* pFinish, br_scalar pLength);

void MakeVertexAndOffsetIt(br_model* pModel, int pVertex_num, br_scalar pX, br_scalar pY, br_scalar pZ, br_vector3* pOffset);

void MakeFaceAndTextureIt(br_model* pModel, int pFace_num, int pV0, int pV1, int pV2, br_material* pMaterial);

void MakeSection(br_uint_16 pFirst_vertex, br_uint_16 pFirst_face, br_vector3* pStart, br_vector3* pFinish, br_scalar pWidth, br_material* pMaterial_centre_lt, br_material* pMaterial_centre_dk, br_material* pMaterial_edges_start_lt, br_material* pMaterial_edges_start_dk, br_material* pMaterial_edges_finish_lt, br_material* pMaterial_edges_finish_dk);

void MakeCube(br_uint_16 pFirst_vertex, br_uint_16 pFirst_face, br_vector3* pPoint, br_material* pMaterial_1, br_material* pMaterial_2, br_material* pMaterial_3);

void CalcNumberOfFacesAndVerticesForOppoPathModel(br_uint_16* pFace_index_ptr, br_uint_16* pVertex_index_ptr);

void ReallocModelFacesAndVertices(br_model* pModel, int pNum_faces, int pNum_vertices);

br_material* CreateSimpleMaterial(int pColour_index);

void AllocateMatsForOppoPathModel(void);

void RebuildOppoPathModel(void);

int ConsistencyCheck(void);

void ShowOppoPaths(void);

void WriteOutOppoPaths(void);

int NewNodeOKHere(void);

void ShowHideOppoPaths(void);

void DropElasticateyNode(void);

void InsertAndElasticate(void);

void InsertAndDontElasticate(void);

void DropDeadEndNode(void);

void DropNodeOnNodeAndStopElasticating(void);

void WidenOppoPathSection(void);

void NarrowOppoPathSection(void);

void IncreaseSectionMinSpeed(void);

void DecreaseSectionMinSpeed(void);

void IncreaseSectionMaxSpeed(void);

void DecreaseSectionMaxSpeed(void);

void PullOppoPoint(void);

void ShowNodeInfo(void);

void ShowSectionInfo1(void);

void ShowSectionInfo2(void);

void DeleteOppoPathSection(void);

void DeleteOppoPathNodeAndSections(void);

void DeleteOppoPathNodeAndJoin(void);

void ReverseSectionDirection(void);

void CycleSectionType(void);

void ToggleOneWayNess(void);

void CopStartPointInfo(void);

void DropCopStartPoint(void);

void DeleteCopStartPoint(void);

void CycleCopStartPointType(void);

#endif
