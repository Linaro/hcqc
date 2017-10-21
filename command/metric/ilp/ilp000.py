import sys

import driver

def get_IPC(bb):
    # TODO
    return "1.3"

class IlpMetricWorker(driver.MetricWorker):
    def match_p(self, target_config, test_name):
        if target_config.compiler == "ClangLLVM":
            return True
        return False

    def set_up_before_getting_data(self, target_config, bb_list):
        pass

    def get_column_name_list(self):
        return ["IPC"]

    def get_data_list(self, target_config, bb):
        ipc = get_IPC(bb)
        return [ipc]

    def get_summary_list(self, target_config):
        return ["-"]

if __name__ == "__main__":
    ilp_metric_worker = IlpMetricWorker()
    driver.run_metric_work_if_match_p(sys.argv, ilp_metric_worker)
